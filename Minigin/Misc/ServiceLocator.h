#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "SoundSystem.h"
#include "RealCollisionSystem.h"

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
	static SoundSystem& GetSoundSystem()
	{
		if (!_ss_instance)
		{
			static NullSoundSystem nullSS;
			return nullSS;
		}

		return *_ss_instance;
	}

	static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss) { _ss_instance = std::move(ss); }

	static CollisionSystem& GetCollisionSystem()
	{
		if (!_cs_instance)
		{
			static NullCollisionSystem nullCS;
			return nullCS;
		}

		return *_cs_instance;
	}
	static void RegisterCollisionSystem(std::unique_ptr<CollisionSystem>&& cs) { _cs_instance = std::move(cs); }
private:
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	static std::unique_ptr<SoundSystem>		_ss_instance;
	static std::unique_ptr<CollisionSystem> _cs_instance;
};
