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
	m_StartCellIndex = m_GridPtr->GetCellIndex({ owner->GetLocalPosition().x, owner->GetLocalPosition().y });
	m_CellIndexBelow = m_StartCellIndex + m_GridPtr->GetColumns();
}

void Rock::HandleCollision(const CollisionEvent& collision)
{
	if (auto newState = m_State->HandleCollision(*this, collision))
	{
		SetState(std::move(newState));
	}
}

void Rock::Update(float deltaTime)
{
	if (auto newState = m_State->Update(*this, deltaTime))
	{
		SetState(std::move(newState));
	}
}

void Rock::SetState(std::unique_ptr<RockStates::RockState> state)
{
	m_State->OnExit(*this);
	m_State = std::move(state);
	m_State->OnEnter(*this);
}
