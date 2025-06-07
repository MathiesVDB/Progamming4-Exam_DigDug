#pragma once
#include <iostream>

class SoundSystem {
public:
	virtual ~SoundSystem() = default;

	// Pure virtual function(s)
	virtual void AddSoundToQueue(const std::string& sound_path) = 0;

	virtual void ToggleMute() = 0;
};

class RealSoundSystem final : public SoundSystem
{
public:
	RealSoundSystem();
	~RealSoundSystem() override;

	//File has to be in the ../Data/SFX/ folder
	void AddSoundToQueue(const std::string& sound_path) override;

	void ToggleMute() override;

private:
	class SoundSystemImpl;
	std::unique_ptr<SoundSystemImpl> m_ImplPtr;
};

class NullSoundSystem : public SoundSystem
{
public:
	void AddSoundToQueue(const std::string& ) override { std::cout << "NULL SOUNDSYSTEM" << std::endl; }

	void ToggleMute() override { std::cout << "NULL SOUNDSYSTEM" << std::endl; };
};
