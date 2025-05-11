#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Component.h"
#include "PookaState.h"
#include "RealCollisionSystem.h"

//-----------------------------------------------------
// Pooka Class									 
//-----------------------------------------------------
class Pooka final : public dae::Component
{
public:
	Pooka(dae::GameObject* owner);

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	void HandleInput();
	void Update(float deltaTime) override;
	void Render() const override;

	void HandleCollision(const CollisionEvent& collision);

	void SetState(PookaState* state);

private:
	PookaState* m_State;
};