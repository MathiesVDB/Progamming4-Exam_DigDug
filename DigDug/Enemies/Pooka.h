#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Component.h"
#include "PookaState.h"

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

	void SetState(PookaState* state);
	void UpdateSpriteForCurrentState();

private:
	PookaState* m_State;
};