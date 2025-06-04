#include "SoundHandler.h"
#include "GameObject.h"
#include "Observer.h"
#include "Player.h"
#include "ServiceLocator.h"

SoundHandler::SoundHandler()
{
	m_SoundMap = {
	{"EnemyDied"		, "EnemyDeath.wav"	},
	{"FygarFire"		, "FygarFire.wav"	},
	{"PlayerHit"		, "PlayerHit.wav"	},
	{"PlayerAttack"		, "PumpShoot.wav"	},
	{"PumpEnemy"		, "PumpToEnemy.wav"	},
	{"Victory"			, "Victory.wav"		},
	{"RockHitEntity"	, "RockHit.wav"		}
	};
}

void SoundHandler::Notify(dae::GameObject* gameObject, dae::EventID event)
{
	std::string eventName = dae::EventRegistry::GetInstance().GetName(event);

	if (eventName == "LifeLost")
	{
		if (gameObject->HasComponent<Player>())
			ServiceLocator::GetSoundSystem().AddSoundToQueue("PlayerDeath.wav");
	}
	else
	{
		auto iterator = m_SoundMap.find(eventName);
		if (iterator != m_SoundMap.end())
		{
			ServiceLocator::GetSoundSystem().AddSoundToQueue(iterator->second);
		}
	}
}
