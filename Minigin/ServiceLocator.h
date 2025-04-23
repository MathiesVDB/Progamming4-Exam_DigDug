#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "SoundSystem.h"

//-----------------------------------------------------
// ServiceLocator Class									 
//-----------------------------------------------------
class ServiceLocator final
{
public:
	ServiceLocator() = default;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	static SoundSystem& get_sound_system() { return *_ss_instance; }
	static void register_sound_system(std::unique_ptr<SoundSystem>&& ss) { _ss_instance = std::move(ss); }

private:
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	static std::unique_ptr<SoundSystem> _ss_instance;
};
