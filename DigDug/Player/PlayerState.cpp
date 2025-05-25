#include "PlayerState.h"

#include "GameObject.h"
#include "GridComponent.h"
#include "Level.h"
#include "Player.h"
#include "Renderer.h"
#include "SpriteComponent.h"

namespace PlayerStates
{
	//-----------------------------------------------------
	// IdleState Class
	//-----------------------------------------------------
	
	void IdleState::Render(const Player& ) const
	{
	}
	
	PlayerStates::PlayerState* IdleState::Update(Player& player, float )
	{
		if (player.GetOwner()->GetVelocity() != glm::vec3{ 0, 0, 0 }) return &PlayerStates::PlayerState::moving;

		return nullptr;
	}
	
	void IdleState::OnEnter(Player& player)
	{
		player.GetOwner()->GetComponent<SpriteComponent>()->SetNewTexture("Sprites/Player/WalkingSprite.png", 1, 8, 0, 0);
	}
	
	void IdleState::OnExit(Player& )
	{
	}
	
	//-----------------------------------------------------
	// MovingState Class
	//-----------------------------------------------------
	
	void MovingState::Render(const Player&) const
	{
	}
	
	PlayerStates::PlayerState* MovingState::Update(Player& player, float)
	{
		if (player.GetOwner()->GetVelocity() == glm::vec3{ 0, 0, 0 }) return &PlayerStates::PlayerState::idling;

		Point2f position{};
		if (player.GetDirection() == MoveDirection::Left || player.GetDirection() == MoveDirection::Up)
		{
			position = { static_cast<float>(player.GetCollider()->GetBoundingBox().x),
							static_cast<float>(player.GetCollider()->GetBoundingBox().y) };
		}
		else
		{
			position = { static_cast<float>(player.GetCollider()->GetBoundingBox().x) + static_cast<float>(player.GetCollider()->GetBoundingBox().w),
							static_cast<float>(player.GetCollider()->GetBoundingBox().y) + static_cast<float>(player.GetCollider()->GetBoundingBox().h) };
		}
		int index{ player.GetGridPtr()->GetCellIndex(position) };

		if (!player.GetGridPtr()->GetGrid()[index].hasBeenDug) return &PlayerStates::PlayerState::digging;

		auto sprite = player.GetOwner()->GetComponent<SpriteComponent>();

		if (player.GetOwner()->GetVelocity().x < 0)
		{
			sprite->SetSpriteBounds(4, 5, true);
		}
		else if (player.GetOwner()->GetVelocity().x > 0)
		{
			sprite->SetSpriteBounds(0, 1, true);
		}
		else if (player.GetOwner()->GetVelocity().y < 0)
		{
			sprite->SetSpriteBounds(2, 3, true);
		}
		else if (player.GetOwner()->GetVelocity().y > 0)
		{
			sprite->SetSpriteBounds(6, 7, true);
		}

		return nullptr;
	}
	
	void MovingState::OnEnter(Player& player)
	{
		player.GetOwner()->GetComponent<SpriteComponent>()->SetNewTexture("Sprites/Player/WalkingSprite.png", 1, 8, 0, 1);
	}
	
	void MovingState::OnExit(Player&)
	{
	}

	//-----------------------------------------------------
	// DiggingState Class
	//-----------------------------------------------------
	
	void DiggingState::Render(const Player& ) const
	{

	}
	
	PlayerStates::PlayerState* DiggingState::Update(Player& player, float )
	{
		if (player.GetOwner()->GetVelocity() == glm::vec3{ 0, 0, 0 }) return &PlayerStates::PlayerState::idling;

		// Bug when player goes down or right, because position point will enter cell last and mess with digging. THis
		if (player.GetDirection() == MoveDirection::Left || player.GetDirection() == MoveDirection::Up)
		{
			m_Position = { static_cast<float>(player.GetCollider()->GetBoundingBox().x),
							static_cast<float>(player.GetCollider()->GetBoundingBox().y)};
		}
		else
		{
			m_Position = { 	static_cast<float>(player.GetCollider()->GetBoundingBox().x) + static_cast<float>(player.GetCollider()->GetBoundingBox().w),
								static_cast<float>(player.GetCollider()->GetBoundingBox().y) + static_cast<float>(player.GetCollider()->GetBoundingBox().h)};
		}
		int index{ player.GetGridPtr()->GetCellIndex(m_Position) };

		if (player.GetGridPtr()->GetGrid()[index].hasBeenDug) return &PlayerStates::PlayerState::moving;

		auto sprite = player.GetOwner()->GetComponent<SpriteComponent>();

		if (player.GetOwner()->GetVelocity().x < 0)
		{
			sprite->SetSpriteBounds(4, 5, true);
		}
		else if (player.GetOwner()->GetVelocity().x > 0)
		{
			sprite->SetSpriteBounds(0, 1, true);
		}
		else if (player.GetOwner()->GetVelocity().y < 0)
		{
			sprite->SetSpriteBounds(2, 3, true);
		}
		else if (player.GetOwner()->GetVelocity().y > 0)
		{
			sprite->SetSpriteBounds(6, 7, true);
		}

		DigCurrentTile(player);

		return nullptr;
	}


	void DiggingState::DigCurrentTile(Player& player) const
	{
		int index{ player.GetGridPtr()->GetCellIndex(m_Position) };

		Point2f playerPos{ m_Position };
		// Spawnpoint always needs to be on player position
		if (player.GetDirection() == MoveDirection::Right || player.GetDirection() == MoveDirection::Down)
		{
			playerPos.x -= player.GetCollider()->GetBoundingBox().w;
			playerPos.y -= player.GetCollider()->GetBoundingBox().h;
		}

		// Already started digging this tile?
		if (!player.GetGridPtr()->GetGrid()[index].coverTile)
		{
			auto emptyTile = player.GetLevelPtr()->SpawnEmpty(playerPos);
			player.GetGridPtr()->GetGrid()[index].coverTile = emptyTile.get();
			player.GetLevelPtr()->GetScene().Add(emptyTile);
		}

		auto coverTile{ player.GetGridPtr()->GetGrid()[index].coverTile };

		if (player.GetDirection() != m_PreviousDirection) return;

		coverTile->GetComponent<dae::Transform>()->SetPosition(playerPos.x, playerPos.y, 0.0f);

		Point2f tileLocation{ coverTile->GetComponent<dae::Transform>()->GetPosition().x, coverTile->GetComponent<dae::Transform>()->GetPosition().y };
		Point2f spawnPosition{ player.GetGridPtr()->GetGrid()[index].spawnPosition.x, player.GetGridPtr()->GetGrid()[index].spawnPosition.y };

		float deltaX = tileLocation.x - spawnPosition.x;
		float deltaY = tileLocation.y - spawnPosition.y;

		float distanceSquared = deltaX * deltaX + deltaY * deltaY;
		float margin = 3.f;

		if (distanceSquared <= margin * margin) player.GetGridPtr()->GetGrid()[index].hasBeenDug = true;
	}
	
	void DiggingState::OnEnter(Player& player)
	{
		player.GetOwner()->GetComponent<SpriteComponent>()->SetNewTexture("Sprites/Player/DiggingSprite.png", 1, 16, 0, 1);

		m_PreviousDirection = player.GetDirection();
	}
	
	void DiggingState::OnExit(Player& )
	{
	}
	
	//-----------------------------------------------------
	// DeathState Class
	//-----------------------------------------------------
	
	void DeathState::Render(const Player& ) const
	{
	}
	
	PlayerStates::PlayerState* DeathState::Update(Player& , float )
	{
		return nullptr;
	}
	
	void DeathState::OnEnter(Player& )
	{
	}
	
	void DeathState::OnExit(Player& )
	{
	}
	
	//-----------------------------------------------------
	// AttackState Class
	//-----------------------------------------------------
	
	void AttackState::Render(const Player& ) const
	{
	}
	
	PlayerStates::PlayerState* AttackState::Update(Player& , float )
	{
		return nullptr;
	}
	
	void AttackState::OnEnter(Player& )
	{
	}
	
	void AttackState::OnExit(Player& )
	{
	}
	
	//-----------------------------------------------------
	// Statics
	//-----------------------------------------------------
	
	IdleState    PlayerState::idling;
	MovingState  PlayerState::moving;
	DiggingState PlayerState::digging;
	DeathState   PlayerState::dying;
	AttackState  PlayerState::attacking;
	
}
