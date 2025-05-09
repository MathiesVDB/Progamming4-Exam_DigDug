#pragma once
#include <iostream>

class SoundSystem {
public:
	virtual ~SoundSystem() = default;

	// Pure virtual function(s)
	virtual void AddSoundToQueue(const std::string& sound_path) = 0;
};

class RealSoundSystem final : public SoundSystem
{
public:
	RealSoundSystem();
	~RealSoundSystem();

	//File has to be in the ../Data/SFX/ folder
	void AddSoundToQueue(const std::string& sound_path) override;

private:
	class SoundSystemImpl;
	std::unique_ptr<SoundSystemImpl> m_ImplPtr;
};

class NullSoundSystem : public SoundSystem
{
public:
	void AddSoundToQueue(const std::string& ) override
	{
		// Do nothing
	}
};
