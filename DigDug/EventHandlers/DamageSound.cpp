#include "DamageSound.h"
#include "Observer.h"
#include "ServiceLocator.h"



DamageSound::DamageSound(const dae::GameObject* player, const std::string& soundPath)
	:	m_Player(player),
		m_SoundPath(soundPath)
{
}

void DamageSound::Notify(const dae::GameObject* gameObject, dae::EventID event)
{
	std::string eventName = dae::EventRegistry::GetInstance().GetName(event);

	if (eventName == "HealthChanged")
	{
		if (gameObject == m_Player)
		{
			ServiceLocator::GetSoundSystem().AddSoundToQueue(m_SoundPath);
		}
	}
}
