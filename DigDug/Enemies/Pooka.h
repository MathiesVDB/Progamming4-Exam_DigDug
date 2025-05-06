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

private:
	std::unique_ptr<PookaState> m_State;
};