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
		: m_Running(true), m_Thread(&SoundSystemImpl::ProcessQueue, this)
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
		}
	}

	~SoundSystemImpl()
	{
		m_Running = false;
		m_Condition.notify_one();
		if (m_Thread.joinable())
			m_Thread.join();
		Mix_CloseAudio();
	}

	void AddSoundToQueue(const std::string& sound_path)
	{
		{
			std::lock_guard<std::mutex> lock(m_QueueMutex);
			m_SoundQueue.push(sound_path);
		}
		m_Condition.notify_one();
	}

private:
	//-------------------------------------------------
	// Datamembers
	//-------------------------------------------------
	std::queue<std::string> m_SoundQueue;
	std::mutex				m_QueueMutex;
	std::condition_variable m_Condition;
	std::thread				m_Thread;
	std::atomic<bool>		m_Running;

	//-------------------------------------------------
	// Member functions
	//-------------------------------------------------
	void ProcessQueue()
	{
		while (m_Running)
		{
			std::unique_lock<std::mutex> lock(m_QueueMutex);
			m_Condition.wait(lock, [this] { return !m_SoundQueue.empty() || !m_Running; });

			while (!m_SoundQueue.empty())
			{
				auto sound_path = m_SoundQueue.front();
				m_SoundQueue.pop();
				lock.unlock(); // unlock while we process

				Mix_Chunk* sound = Mix_LoadWAV(sound_path.c_str());
				if (!sound)
				{
					std::cerr << "Failed to load sound: " << sound_path << " Error: " << Mix_GetError() << std::endl;
				}
				else
				{
					Mix_PlayChannel(-1, sound, 0); // play once on any free channel
					// Let SDL_mixer handle cleanup later, or you can delay and free
					std::this_thread::sleep_for(std::chrono::milliseconds(100)); // simulate play duration buffer
					Mix_FreeChunk(sound);
				}
				lock.lock();
			}
		}
	}
};

SoundSystem::SoundSystem() : m_ImplPtr(std::make_unique<SoundSystemImpl>())
{
}

void SoundSystem::AddSoundToQueue(const std::string& sound_path)
{
	m_ImplPtr->AddSoundToQueue(sound_path);
}