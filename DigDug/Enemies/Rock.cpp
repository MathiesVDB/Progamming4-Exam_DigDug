//---------------------------
// Include Files
//---------------------------
#include "Rock.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "Transform.h"

//---------------------------
// Constructor & Destructor
//---------------------------
Rock::Rock(dae::GameObject* owner, GridComponent* grid)
	:	Component(owner),
		m_GridPtr{ grid }
{
	int cellIndex = m_GridPtr->GetCellIndex({ owner->GetLocalPosition().x, owner->GetLocalPosition().y });

	m_StartCell = m_GridPtr->GetGrid()[cellIndex];
	m_CellBelow = m_GridPtr->GetGrid()[cellIndex + GridComponent::COLUMNS];
}

void Rock::HandleCollision(const CollisionEvent& collision)
{
	auto newState = m_State->HandleCollision(*this, collision);

	if (newState == nullptr) return;
	SetState(newState);
}

void Rock::Update(float deltaTime)
{
	auto newState = m_State->Update(*this, deltaTime);

	if (newState == nullptr) return;
	SetState(newState);
}

void Rock::SetState(RockStates::RockState* state)
{
	if (m_State == state) return;

	m_State->OnExit(*this);
	m_State = state;
	m_State->OnEnter(*this);
}
