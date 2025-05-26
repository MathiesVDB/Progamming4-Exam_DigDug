#include "RockState.h"
#include "Rock.h"
#include "SpriteComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "SceneManager.h"

using namespace RockStates;

//-----------------------------------------------------
// Idle state
//-----------------------------------------------------

RockStates::RockState* IdleState::HandleCollision(Rock& , const CollisionEvent& )
{
	return nullptr;
}

RockStates::RockState* IdleState::Update(Rock& rock, float )
{
	auto cellBelow{ rock.GetGridPtr()->GetGrid()[rock.GetCellIndexBelow()] };

	if (cellBelow.hasBeenDug)
	{
		return &RockStates::RockState::tilting;
	}

	return nullptr;
}

void IdleState::OnEnter(Rock& rock)
{
	m_Sprite = rock.GetOwner()->GetComponent<SpriteComponent>();
	m_Sprite->SetSpriteBounds(0, 0, true);
}

void IdleState::OnExit(Rock& )
{
	
}

//-----------------------------------------------------
// Tilt state
//-----------------------------------------------------

RockStates::RockState* TiltState::HandleCollision(Rock&, const CollisionEvent&)
{
	return nullptr;
}

RockStates::RockState* TiltState::Update(Rock& rock, float )
{
	auto playerPosition = glm::vec2{ m_Player->GetWorldPosition() };
	//Make sure player is completely away from under the rock before falling
	int playerTopLeftCellIndex{ rock.GetGridPtr()->GetCellIndex(playerPosition) };

	glm::vec2 playerBottomRightPos{
		playerPosition.x + static_cast<float>(m_PlayerCollider->GetBoundingBox().w),
		playerPosition.y + static_cast<float>(m_PlayerCollider->GetBoundingBox().h)
	};

	int playerBottomRightCellIndex{ rock.GetGridPtr()->GetCellIndex(playerBottomRightPos) };

	if (rock.GetCellIndexBelow() != playerTopLeftCellIndex && rock.GetCellIndexBelow() != playerBottomRightCellIndex) return &RockStates::RockState::falling;

	return nullptr;
}

void TiltState::OnEnter(Rock& rock)
{
	m_Sprite = rock.GetOwner()->GetComponent<SpriteComponent>();
	m_Sprite->SetSpriteBounds(0, 1, true);

	m_Player = dae::SceneManager::GetInstance().GetActiveScene().GetPlayer(0);
	m_PlayerCollider = m_Player->GetComponent<ColliderComponent>();
}

void TiltState::OnExit(Rock&)
{

}

//-----------------------------------------------------
// Fall state
//-----------------------------------------------------

RockStates::RockState* FallState::HandleCollision(Rock& rock, const CollisionEvent& collision)
{
	auto grid{ rock.GetGridPtr() };
	const auto& colliderTag{ collision.collider->GetComponent<ColliderComponent>()->GetTag() };
	const auto& collidedTag{ collision.collided->GetComponent<ColliderComponent>()->GetTag() };

	int index{};

	if		(colliderTag == Tag::GROUND) index = grid->GetCellIndex(glm::vec2{ collision.collider->GetLocalPosition() });
	else if (collidedTag == Tag::GROUND) index = grid->GetCellIndex(glm::vec2{ collision.collided->GetLocalPosition() });
	else return nullptr;

	if (index == rock.GetStartCellIndex() || grid->GetGrid()[index].hasBeenDug) return nullptr;

	return &RockStates::RockState::breaking;
}

RockStates::RockState* FallState::Update(Rock& rock, float deltaTime)
{
	auto rockVelocity = GRAVITY * deltaTime;

	rock.GetOwner()->AddVelocity({ 0, rockVelocity });

	return nullptr;
}

void FallState::OnEnter(Rock& rock)
{
	m_Sprite = rock.GetOwner()->GetComponent<SpriteComponent>();
	m_Sprite->SetSpriteBounds(0, 0, true);
}

void FallState::OnExit(Rock&)
{

}

//-----------------------------------------------------
// Break state
//-----------------------------------------------------

RockStates::RockState* BreakState::HandleCollision(Rock&, const CollisionEvent&)
{
	return nullptr;
}

RockStates::RockState* BreakState::Update(Rock& rock, float)
{
	if(m_Sprite->HasReachedLastframe()) dae::SceneManager::GetInstance().GetActiveScene().MarkForDeletion(rock.GetOwner());

	return nullptr;
}

void BreakState::OnEnter(Rock& rock)
{
	m_Sprite = rock.GetOwner()->GetComponent<SpriteComponent>();
	m_Sprite->SetSpriteBounds(0, 3, true);
}

void BreakState::OnExit(Rock&)
{

}

//-----------------------------------------------------
// Statics
//-----------------------------------------------------

IdleState		RockState::idling;
TiltState		RockState::tilting;
FallState		RockState::falling;
BreakState      RockState::breaking;