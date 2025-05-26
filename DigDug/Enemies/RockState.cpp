#include "RockState.h"
#include "Rock.h"
#include "SpriteComponent.h"
#include "GameObject.h"

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
		return &RockStates::RockState::falling;
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

RockStates::RockState* TiltState::Update(Rock& , float )
{
	return nullptr;
}

void TiltState::OnEnter(Rock& rock)
{
	m_Sprite = rock.GetOwner()->GetComponent<SpriteComponent>();
	m_Sprite->SetSpriteBounds(0, 1, true);
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

	if		(colliderTag == Tag::GROUND) index = grid->GetCellIndex({ collision.collider->GetLocalPosition().x, collision.collider->GetLocalPosition().y });
	else if (collidedTag == Tag::GROUND) index = grid->GetCellIndex({ collision.collided->GetLocalPosition().x, collision.collided->GetLocalPosition().y });
	else return nullptr;

	if (index == rock.GetStartCellIndex() || grid->GetGrid()[index].hasBeenDug) return nullptr;

	return &RockStates::RockState::breaking;
}

RockStates::RockState* FallState::Update(Rock& rock, float deltaTime)
{
	auto rockVelocity = GRAVITY * deltaTime;

	rock.GetOwner()->AddVelocity({ 0, rockVelocity, 0 });

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

RockStates::RockState* BreakState::Update(Rock& , float)
{
	// If break animation is done, remove rock
	// if(m_Sprite->HasReachedMaxFrame()) SceneManager::GetInstance().GetCurrentScene()->Remove(rock.GetOwner());

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