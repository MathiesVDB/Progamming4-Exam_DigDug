#include "SelfDestructComponent.h"

#include "Scene.h"
#include "SceneManager.h"

SelfDestructComponent::SelfDestructComponent(dae::GameObject* owner, float timeToLive)
	:	Component(owner),
		m_TimeToLive{timeToLive}
{
}

void SelfDestructComponent::Update(float deltaTime)
{
	m_AccumulatedTime += deltaTime;

	if (m_AccumulatedTime >= m_TimeToLive) dae::SceneManager::GetInstance().GetActiveScene().MarkForDeletion(GetOwner());
}