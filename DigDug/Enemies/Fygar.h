#pragma once
#include <vec2.hpp>
#include "RealCollisionSystem.h"
#include "Level.h"
#include "FygarState.h"

//-----------------------------------------------------
// Fygar Class									 
//-----------------------------------------------------

class Fygar final : public dae::Component, public dae::Subject
{
public:
	Fygar(dae::GameObject* owner, GridComponent* grid, bool isControlled = false);

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	void Update(float deltaTime) override;
	void Attack();

	void HandleCollision(const CollisionEvent& collision);
	RopeComponent* GetRopeFromCollision(const CollisionEvent& collision);

	void SetState(std::unique_ptr<FygarStates::FygarState> newState);
	void ResetFygar();

	void IncreaseInflation();
	void DecreaseInflation();

	void ToggleFleeing() { m_IsFleeing = !m_IsFleeing; }

	// Event notifiers
	void NotifyDeath()	const;
	void NotifyAttack() const;

	// Getters for state machine
	GridComponent* GetGridPtr() const { return m_GridPtr; }

	Inflated GetInflatedState() const { return m_InflatedState; }

	const glm::vec2& GetTarget()		const { return m_CurrentTarget; }
	const glm::vec2& GetSpawnPosition() const { return m_SpawnPosition; }

	bool WasCrushed()	 const { return m_WasCrushed; }
	bool IsLookingLeft() const { return m_IsLookingLeft; }
	bool IsFleeing()	 const { return m_IsFleeing; }
	bool IsDeadly()		 const;
	bool IsControlled()  const { return m_IsControlled; }
	bool CanMove()		 const;

	//Movement functions
	bool CanSwitchMovement(MoveDirection direction);
	bool CanMoveHorizontal(const SDL_Rect& boundingBox);
	bool CanMoveVertical(const SDL_Rect& boundingBox);
	bool IsVertical(MoveDirection dir) const;
	bool IsHorizontal(MoveDirection dir) const;
	void SnapToCellCenter();

	//Constants
	static const int SNAP_DISTANCE{ 1 };

private:
	GridComponent* m_GridPtr;

	std::unique_ptr<FygarStates::FygarState> m_State;
	Inflated m_InflatedState{ Inflated::None };

	glm::vec2 m_FleeingTarget{};
	glm::vec2 m_CurrentTarget{};
	glm::vec2 m_SpawnPosition{}; // Save for level reset

	MoveDirection m_Direction;

	bool m_IsLookingLeft{ false };
	bool m_WasCrushed	{ false };
	bool m_IsFleeing	{ false };
	bool m_IsControlled;
	bool m_IsDeflating	{ false };
	bool m_HasRegisteredCrush{ false };
};
