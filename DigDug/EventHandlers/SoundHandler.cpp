#include "SoundHandler.h"
#include "GameObject.h"
#include "Observer.h"
#include "Player.h"
#include "ServiceLocator.h"

SoundHandler::SoundHandler() = default;

void SoundHandler::Notify(const dae::GameObject* gameObject, dae::EventID event)
{
	static const std::unordered_map<std::string, std::string> soundMap = {
		{"EnemyDied"		, "EnemyDeath.wav"	},
		{"FygarFire"		, "FygarFire.wav"	},
		{"PlayerHit"		, "PlayerHit.wav"	},
		{"PlayerAttack"	, "PumpShoot.wav"	},
		{"PumpEnemy"		, "PumpToEnemy.wav"	},
		{"Victory"		, "Victory.wav"		},
		{"RockHitEntity"	, "RockHit.wav"		}
	};

	std::string eventName = dae::EventRegistry::GetInstance().GetName(event);

	if (eventName == "LifeLost")
	{
		if (gameObject->HasComponent<Player>())
			ServiceLocator::GetSoundSystem().AddSoundToQueue("PlayerDeath.wav");
	}
	else
	{
		auto iterator = soundMap.find(eventName);
		if (iterator != soundMap.end())
		{
			ServiceLocator::GetSoundSystem().AddSoundToQueue(iterator->second);
		}
	}
}
