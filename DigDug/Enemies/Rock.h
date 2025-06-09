#pragma once
//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Component.h"
#include "GridComponent.h"
#include "RealCollisionSystem.h"
#include "RockState.h"

namespace dae
{
	class Scene;
}

class GridComponent;
//-----------------------------------------------------
// Rock Class									 
//-----------------------------------------------------
class Rock final : public dae::Component
{
public:
	Rock(dae::GameObject* owner, GridComponent* grid);

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	void HandleCollision(const CollisionEvent& collision);
	void Update(float deltaTime) override;

	void SetState(std::unique_ptr<RockStates::RockState> state);

	GridComponent* GetGridPtr()	const { return m_GridPtr; }
	int GetStartCellIndex()		const { return m_StartCellIndex; }
	int GetCellIndexBelow()		const { return m_CellIndexBelow; }
	bool IsFalling()			const { return dynamic_cast<RockStates::FallState*>(m_State.get()) != nullptr;}
	bool IsBreaking()			const { return dynamic_cast<RockStates::BreakState*>(m_State.get()) != nullptr; }

private:
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	int m_StartCellIndex;
	int m_CellIndexBelow;

	GridComponent* m_GridPtr;
	std::unique_ptr<RockStates::RockState> m_State{std::make_unique<RockStates::IdleState>() };
};
