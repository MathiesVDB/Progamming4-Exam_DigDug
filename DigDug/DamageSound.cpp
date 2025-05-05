#include "DamageSound.h"

#include "ServiceLocator.h"

DamageSound::DamageSound(const dae::GameObject* player, const std::string& soundPath)
	:	m_Player(player),
		m_SoundPath(soundPath)
{
}

void DamageSound::Notify(const dae::GameObject* gameObject, Event event)
{
	if (event == Event::HealthChanged)
	{
		if (gameObject == m_Player)
		{
			ServiceLocator::GetSoundSystem().AddSoundToQueue(m_SoundPath);
		}
	}
}
