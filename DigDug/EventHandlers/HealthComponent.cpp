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

void HealthComponent::TakeDamage()
{
	dae::EventID LifeLostEventID = dae::EventRegistry::GetInstance().GetEventID("LifeLost");

	--m_Lives;
	Notify(GetOwner(), LifeLostEventID);
}

void HealthComponent::SetLivesToMax()
{
	dae::EventID HealthIncreaseEventID = dae::EventRegistry::GetInstance().GetEventID("HealthIncrease");

	m_Lives = m_MaxLives;
	Notify(GetOwner(), HealthIncreaseEventID);
}