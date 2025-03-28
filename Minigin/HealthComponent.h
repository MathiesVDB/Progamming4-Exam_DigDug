#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Component.h"
#include "Observer.h"

//-----------------------------------------------------
// HealthComponent Class									 
//-----------------------------------------------------
class HealthComponent final : public Component, public Subject
{
public:
	HealthComponent(int maxLives);

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	HealthComponent(const HealthComponent& other)					= default;
	HealthComponent(HealthComponent&& other) noexcept				= default;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	int GetLives() const;

	void SetMaxLives(int newMaxLives);
	void TakeDamage(int takenDamage);

	void SetLivesToMax();

private:
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	int m_Lives{};
	int m_MaxLives;
};
