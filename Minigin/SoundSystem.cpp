#include "SoundSystem.h"
#include <SDL_mixer.h>
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

class SoundSystem::SoundSystemImpl
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
	}

	~SoundSystemImpl()
	{
		m_Running = false;
		m_Condition.notify_all();
		if (m_WorkerThread.joinable())
			m_WorkerThread.join();

		Mix_CloseAudio();
	}

	SoundSystemImpl(const SoundSystemImpl& other)					= delete;
	SoundSystemImpl(SoundSystemImpl&& other) noexcept				= delete;
	SoundSystemImpl& operator=(const SoundSystemImpl& other)		= delete;
	SoundSystemImpl& operator=(SoundSystemImpl&& other) noexcept	= delete;

	//File has to be in the ../Data/SFX/ folder
	void AddSoundToQueue(const std::string& soundPath) 
	{
		{
			std::string fullPath = "../Data/SFX/" + soundPath;
			std::cout << "[Sound System] Adding sound to queue: " << fullPath << std::endl;
			std::lock_guard<std::mutex> lock(m_QueueMutex);
			m_SoundQueue.push(fullPath);
		}
		m_Condition.notify_one();
	}

private:
	std::queue<std::string>		m_SoundQueue;
	std::mutex					m_QueueMutex;
	std::condition_variable		m_Condition;
	std::atomic<bool>			m_Running;
	std::jthread				m_WorkerThread;

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

		// Wait until the sound finishes playing
		while (Mix_Playing(channel) != 0 && m_Running)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}

		Mix_FreeChunk(sound);
	}
};

SoundSystem::SoundSystem() : m_ImplPtr(std::make_unique<SoundSystemImpl>())
{
}

SoundSystem::~SoundSystem() = default;

void SoundSystem::AddSoundToQueue(const std::string& sound_path)
{
	m_ImplPtr->AddSoundToQueue(sound_path);
}