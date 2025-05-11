//---------------------------
// Include Files
//---------------------------
#include "HealthComponent.h"
#include "Observer.h"
#include "GameObject.h"

//---------------------------
// Constructor & Destructor
//---------------------------
HealthComponent::HealthComponent(dae::GameObject* owner, int maxLives)
	:	Component(owner),
		m_MaxLives{ maxLives }
{
	SetLivesToMax();
}

//---------------------------
// Member functions
//---------------------------

int HealthComponent::GetLives() const
{
	return m_Lives;
}

void HealthComponent::SetMaxLives(int newMaxLives)
{
	m_MaxLives = newMaxLives;
}

void HealthComponent::TakeDamage(int takenDamage)
{
	dae::EventID HealthChangedEventID = dae::EventRegistry::GetInstance().GetEventID("HealthChanged");

	m_Lives -= takenDamage;
	Notify(GetOwner(), HealthChangedEventID);
}

void HealthComponent::SetLivesToMax()
{
	dae::EventID HealthChangedEventID = dae::EventRegistry::GetInstance().GetEventID("HealthChanged");

	m_Lives = m_MaxLives;
	Notify(GetOwner(), HealthChangedEventID);
}