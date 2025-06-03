#include "SoundHandler.h"
#include "GameObject.h"
#include "Observer.h"
#include "Player.h"
#include "ServiceLocator.h"

SoundHandler::SoundHandler()
{
}

void SoundHandler::Notify(const dae::GameObject* gameObject, dae::EventID event)
{
	std::string eventName = dae::EventRegistry::GetInstance().GetName(event);

	if (eventName == "LifeLost")
	{
		if (gameObject->HasComponent<Player>()) ServiceLocator::GetSoundSystem().AddSoundToQueue("PlayerHit.wav");
	}
}
