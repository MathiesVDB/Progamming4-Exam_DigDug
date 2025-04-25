#pragma once
#include <iostream>

class SoundSystem final
{
public:
	SoundSystem();
	~SoundSystem();

	//File has to be in the ../Data/SFX/ folder
	void AddSoundToQueue(const std::string& sound_path);

private:
	class SoundSystemImpl;
	std::unique_ptr<SoundSystemImpl> m_ImplPtr;
};