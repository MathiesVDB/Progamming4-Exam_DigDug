#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Component.h"
#include "Observer.h"

//-----------------------------------------------------
// HealthComponent Class									 
//-----------------------------------------------------
class HealthComponent final : public dae::Component, public dae::Subject
{
public:
	HealthComponent(dae::GameObject* owner, int maxLives);

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	HealthComponent(const HealthComponent& other) = default;
	HealthComponent(HealthComponent&& other) noexcept = default;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	int GetLives() const;

	void SetMaxLives(int newMaxLives);
	void TakeDamage(); //Nothing in the game removes more than one health so just remove 1

	void SetLivesToMax();

private:
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	int m_Lives{};
	int m_MaxLives;
};
