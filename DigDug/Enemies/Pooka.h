#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Component.h"
#include "Helpers.h"
#include "PookaState.h"
#include "RealCollisionSystem.h"

class GridComponent;
//-----------------------------------------------------
// Pooka Class									 
//-----------------------------------------------------
class Pooka final : public dae::Component
{
public:
	Pooka(dae::GameObject* owner, GridComponent* grid);

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	void Update(float deltaTime) override;
	void Render() const override;

	void HandleCollision(const CollisionEvent& collision);

	void SetState(std::unique_ptr<PookaStates::PookaState> newState);

	void IncreaseInflation();
	void ResetInflation();

	// Getters for state machine
	GridComponent* GetGridPtr() const { return m_GridPtr; }

	Inflated GetInflatedState() const { return m_InflatedState; }

	const glm::vec2& GetTarget()		const  { return m_CurrentTarget; }
	const glm::vec2& GetSpawnPosition() const  { return m_SpawnPosition; }

	bool WasCrushed()	 const	{ return m_WasCrushed; }
	bool IsLookingLeft() const	{ return m_IsLookingLeft; }
	bool IsFleeing()	 const	{ return m_IsFleeing; }
	bool IsDeadly()		 const  { return dynamic_cast<PookaStates::MovingState*>(m_State.get()) != nullptr; }

private:
	GridComponent* m_GridPtr;

	std::unique_ptr<PookaStates::PookaState> m_State;
	Inflated m_InflatedState{ Inflated::None };

	glm::vec2 m_FleeingTarget{};
	glm::vec2 m_CurrentTarget{};
	glm::vec2 m_SpawnPosition{}; // Save for level reset

	bool m_IsLookingLeft{ false };
	bool m_WasCrushed	{ false };
	bool m_IsFleeing	{ false };
};