#pragma once
#include <algorithm>
#include <iostream>

using sound_id = unsigned short;
class sound_system
{
public:
	virtual ~sound_system() = default;
	virtual void play(const sound_id id, const float volume) = 0;
};

class servicelocator final
{
	static std::unique_ptr<sound_system> _ss_instance;
public:
	static sound_system& get_sound_system() { return *_ss_instance; }
	static void register_sound_system(std::unique_ptr<sound_system>&& ss) { _ss_instance = std::move(ss); }
};

class sdl_sound_system final : public sound_system
{
public:
	void play(const sound_id id, const float volume) override
	{
		// lots of sdl_mixer code
	}
};

class logging_sound_system final : public sound_system {
	std::unique_ptr<sound_system> _real_ss;
public:
	logging_sound_system(std::unique_ptr<sound_system>&& ss) : _real_ss(std::move(ss)) {}
	virtual ~logging_sound_system() = default;
	void play(const sound_id id, const float volume) override {
		_real_ss->play(id, volume);
		std::cout << "playing " << id << " at volume " << volume << std::endl;
	}
};