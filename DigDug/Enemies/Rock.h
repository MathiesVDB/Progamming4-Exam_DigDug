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

	GridComponent* GetGridPtr()			const { return m_GridPtr; }
	GridComponent::Cell GetStartCell()	const { return m_StartCell; }
	GridComponent::Cell GetCellBelow()	const { return m_CellBelow; }

private:
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	GridComponent::Cell m_StartCell;
	GridComponent::Cell m_CellBelow;

	GridComponent* m_GridPtr;
	RockStates::RockState* m_State{ &RockStates::RockState::idling };
};
