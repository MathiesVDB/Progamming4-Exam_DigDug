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

std::unique_ptr<RockStates::RockState> IdleState::HandleCollision(Rock& , const CollisionEvent& )
{
	return nullptr;
}

std::unique_ptr<RockStates::RockState> IdleState::Update(Rock& rock, float )
{
	auto cellBelow{ rock.GetGridPtr()->GetGrid()[rock.GetCellIndexBelow()] };

	if (cellBelow.hasBeenDug)
	{
		return std::make_unique<TiltState>();
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

std::unique_ptr<RockStates::RockState> TiltState::HandleCollision(Rock&, const CollisionEvent&)
{
	return nullptr;
}

std::unique_ptr<RockStates::RockState> TiltState::Update(Rock& rock, float )
{
	for (const auto& player : m_Players)
	{
		const glm::vec2 playerPosition = player->GetWorldPosition();
		const auto collider = player->GetComponent<ColliderComponent>();

		int playerTopLeftCellIndex = rock.GetGridPtr()->GetCellIndex(playerPosition);

		glm::vec2 playerBottomRightPos{
			playerPosition.x + static_cast<float>(collider->GetBoundingBox().w),
			playerPosition.y + static_cast<float>(collider->GetBoundingBox().h)
		};

		int playerBottomRightCellIndex = rock.GetGridPtr()->GetCellIndex(playerBottomRightPos);

		if (rock.GetCellIndexBelow() == playerTopLeftCellIndex || rock.GetCellIndexBelow() == playerBottomRightCellIndex)
		{
			return nullptr;
		}
	}

	return std::make_unique<FallState>();
}

void TiltState::OnEnter(Rock& rock)
{
	m_Sprite = rock.GetOwner()->GetComponent<SpriteComponent>();
	m_Sprite->SetSpriteBounds(0, 1, true);

	const auto& players{ dae::SceneManager::GetInstance().GetActiveScene().GetObjectsByTag("Player") };

	assert(!players.empty() && "NO PLAYERS FOUND");

	m_Players = players;
}

void TiltState::OnExit(Rock&)
{

}

//-----------------------------------------------------
// Fall state
//-----------------------------------------------------

std::unique_ptr<RockStates::RockState> FallState::HandleCollision(Rock& rock, const CollisionEvent& collision)
{
	auto grid{ rock.GetGridPtr() };
	const auto& colliderTag{ collision.collider->GetComponent<ColliderComponent>()->GetTag() };
	const auto& collidedTag{ collision.collided->GetComponent<ColliderComponent>()->GetTag() };

	int index{};

	if		(colliderTag == Tag::GROUND) index = grid->GetCellIndex(glm::vec2{ collision.collider->GetLocalPosition() });
	else if (collidedTag == Tag::GROUND) index = grid->GetCellIndex(glm::vec2{ collision.collided->GetLocalPosition() });
	else
	{
		auto colliderHeight{ rock.GetColliderPtr()->GetBoundingBox().h };

		glm::vec2 bottomRock{ rock.GetOwner()->GetWorldPosition().x, rock.GetOwner()->GetWorldPosition().y + colliderHeight };
		index = grid->GetCellIndex(bottomRock);
		if (index > -1 && index < 238) return nullptr;
	}

	if (index == rock.GetStartCellIndex() || (grid->GetGrid()[index].hasBeenDug && index > -1 && index < 238)) return nullptr;

	return std::make_unique<BreakState>();
}

std::unique_ptr<RockStates::RockState> FallState::Update(Rock& rock, float deltaTime)
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

std::unique_ptr<RockStates::RockState> BreakState::HandleCollision(Rock&, const CollisionEvent&)
{
	return nullptr;
}

std::unique_ptr<RockStates::RockState> BreakState::Update(Rock& rock, float)
{
	if(m_Sprite->HasReachedLastframe()) dae::SceneManager::GetInstance().GetActiveScene().MarkForDeletion(rock.GetOwner());

	return nullptr;
}

void BreakState::OnEnter(Rock& rock)
{
	rock.NotifyBreak();

	m_Sprite = rock.GetOwner()->GetComponent<SpriteComponent>();
	m_Sprite->SetSpriteBounds(0, 3, true);
}

void BreakState::OnExit(Rock&)
{

}