#pragma once
#include "Component.h"

//-----------------------------------------------------------------
// Use this component to create temporary gameobjects
//-----------------------------------------------------------------

class SelfDestructComponent : public dae::Component
{
public:
	SelfDestructComponent(dae::GameObject* owner, float timeToLive);

	void Update(float deltaTime) override;

private:
	float m_AccumulatedTime{};
	float m_TimeToLive;
};

