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

	void SetState(RockStates::RockState* state);

	GridComponent* GetGridPtr()	const { return m_GridPtr; }
	int GetStartCellIndex()		const { return m_StartCellIndex; }
	int GetCellIndexBelow()		const { return m_CellIndexBelow; }
	bool IsFalling()			const { return m_State == &RockStates::RockState::falling; }

private:
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	int m_StartCellIndex;
	int m_CellIndexBelow;

	GridComponent* m_GridPtr;
	RockStates::RockState* m_State{ &RockStates::RockState::idling };
};
