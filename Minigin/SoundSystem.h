#pragma once
#include <iostream>

class SoundSystem final
{
public:
	SoundSystem();
	~SoundSystem() = default;

	void AddSoundToQueue(const std::string& sound_path);

private:
	class SoundSystemImpl;
	std::unique_ptr<SoundSystemImpl> m_ImplPtr;
};