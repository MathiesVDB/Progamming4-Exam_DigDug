#include "SoundSystem.h"
#include <SDL_mixer.h>
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <unordered_map>

class RealSoundSystem::SoundSystemImpl
{
public:
	SoundSystemImpl()
		: m_Running(true), m_WorkerThread(&SoundSystemImpl::Run, this)
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			std::cerr << "[Sound System] SDL_mixer could not initialize! Error: " << Mix_GetError() << std::endl;
		}
		else
		{
			std::cout << "[Sound System] SDL_mixer initialized successfully." << std::endl;
		}

		s_Instance = this;
		Mix_ChannelFinished(ChannelDoneCallback);
	}

	~SoundSystemImpl()
	{
		m_Running = false;
		m_Condition.notify_all();
		if (m_WorkerThread.joinable())
			m_WorkerThread.join();

		{
			std::lock_guard<std::mutex> lock(m_ChannelMapMutex);
			for (auto& [channel, chunk] : m_ActiveSounds)
			{
				Mix_FreeChunk(chunk);
			}
			m_ActiveSounds.clear();
		}

		Mix_CloseAudio();
	}

	SoundSystemImpl(const SoundSystemImpl& other)					= delete;
	SoundSystemImpl(SoundSystemImpl&& other) noexcept				= delete;
	SoundSystemImpl& operator=(const SoundSystemImpl& other)		= delete;
	SoundSystemImpl& operator=(SoundSystemImpl&& other) noexcept	= delete;

	void AddSoundToQueue(const std::string& soundPath) 
	{
		if (m_IsMuted)
		{
			std::cout << "[Sound System] NO SOUND ADDED - SOUNDSYSTEM IS MUTED" << std::endl;
			return;
		}

		{
			std::string fullPath = "../Data/SFX/" + soundPath;
			std::cout << "[Sound System] Adding sound to queue: " << fullPath << std::endl;
			std::lock_guard<std::mutex> lock(m_QueueMutex);
			m_SoundQueue.push(fullPath);
		}
		m_Condition.notify_one();
	}

	void ToggleMute() { m_IsMuted = !m_IsMuted; }

private:
	std::queue<std::string>		m_SoundQueue;
	std::mutex					m_QueueMutex;
	std::condition_variable		m_Condition;
	std::atomic<bool>			m_Running;
	std::jthread				m_WorkerThread;

	std::mutex m_ChannelMapMutex;
	std::unordered_map<int, Mix_Chunk*> m_ActiveSounds;

	bool m_IsMuted{ false };

	static SoundSystemImpl* s_Instance;

	void Run()
	{
		while (m_Running)
		{
			std::unique_lock<std::mutex> lock(m_QueueMutex);
			m_Condition.wait(lock, [this] { return !m_SoundQueue.empty() || !m_Running; });

			while (!m_SoundQueue.empty())
			{
				std::string soundPath = m_SoundQueue.front();
				m_SoundQueue.pop();
				lock.unlock();

				PlaySound(soundPath);

				lock.lock();
			}
		}
	}

	void PlaySound(const std::string& path)
	{
		Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
		if (!sound)
		{
			std::cerr << "[Sound System] Failed to load sound: " << path << " Error: " << Mix_GetError() << std::endl;
			return;
		}

		std::cout << "[Sound System] Playing sound: " << path << std::endl;
		int channel = Mix_PlayChannel(-1, sound, 0);

		if (channel == -1)
		{
			std::cerr << "[Sound System] Failed to play sound: " << path << " Error: " << Mix_GetError() << std::endl;
			Mix_FreeChunk(sound);
			return;
		}

		{
			std::lock_guard<std::mutex> lock(m_ChannelMapMutex);
			m_ActiveSounds[channel] = sound;
		}
	}

	static void ChannelDoneCallback(int channel)
	{
		std::lock_guard<std::mutex> lock(s_Instance->m_ChannelMapMutex);
		auto it = s_Instance->m_ActiveSounds.find(channel);
		if (it != s_Instance->m_ActiveSounds.end())
		{
			Mix_FreeChunk(it->second);
			s_Instance->m_ActiveSounds.erase(it);
			std::cout << "[Sound System] Freed sound from channel " << channel << std::endl;
		}
	}
};

RealSoundSystem::SoundSystemImpl* RealSoundSystem::SoundSystemImpl::s_Instance = nullptr;

RealSoundSystem::RealSoundSystem() : m_ImplPtr(std::make_unique<SoundSystemImpl>())
{
}

RealSoundSystem::~RealSoundSystem() = default;

void RealSoundSystem::AddSoundToQueue(const std::string& sound_path)
{
	m_ImplPtr->AddSoundToQueue(sound_path);
}

void RealSoundSystem::ToggleMute()
{
	m_ImplPtr->ToggleMute();
}
