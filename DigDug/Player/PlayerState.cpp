#include "PlayerState.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "Level.h"
#include "Player.h"
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

		switch (player.GetDirection())
		{
		case MoveDirection::Left:
			player.GetOwner()->GetComponent<SpriteComponent>()->SetSpriteBounds(4, 4, true);
			break;
		case MoveDirection::Right:
			player.GetOwner()->GetComponent<SpriteComponent>()->SetSpriteBounds(0, 0, true);
			break;
		case MoveDirection::Up:
			player.GetOwner()->GetComponent<SpriteComponent>()->SetSpriteBounds(2, 2, true);
			break;
		case MoveDirection::Down:
			player.GetOwner()->GetComponent<SpriteComponent>()->SetSpriteBounds(6, 6, true);
			break;
		}

		return nullptr;
	}
	
	void IdleState::OnEnter(Player& player)
	{
		m_Sprite = player.GetOwner()->GetComponent<SpriteComponent>();
		m_Sprite->SetNewTexture("Sprites/Player/WalkingSprite.png", 1, 8, 0, 0);
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

		if (player.GetOwner()->GetVelocity().x < 0)
		{
			m_Sprite->SetSpriteBounds(4, 5, true);
		}
		else if (player.GetOwner()->GetVelocity().x > 0)
		{
			m_Sprite->SetSpriteBounds(0, 1, true);
		}
		else if (player.GetOwner()->GetVelocity().y < 0)
		{
			m_Sprite->SetSpriteBounds(2, 3, true);
		}
		else if (player.GetOwner()->GetVelocity().y > 0)
		{
			m_Sprite->SetSpriteBounds(6, 7, true);
		}

		return nullptr;
	}
	
	void MovingState::OnEnter(Player& player)
	{
		m_Sprite = player.GetOwner()->GetComponent<SpriteComponent>();
		m_Sprite->SetNewTexture("Sprites/Player/WalkingSprite.png", 1, 8, 0, 1);
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

		if (player.GetOwner()->GetVelocity().x < 0)
		{
			m_Sprite->SetSpriteBounds(4, 5, true);
		}
		else if (player.GetOwner()->GetVelocity().x > 0)
		{
			m_Sprite->SetSpriteBounds(0, 1, true);
		}
		else if (player.GetOwner()->GetVelocity().y < 0)
		{
			m_Sprite->SetSpriteBounds(2, 3, true);
		}
		else if (player.GetOwner()->GetVelocity().y > 0)
		{
			m_Sprite->SetSpriteBounds(6, 7, true);
		}

		DigCurrentTile(player);

		return nullptr;
	}


	void DiggingState::DigCurrentTile(Player& player)
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
			player.GetGridPtr()->GetGrid()[index].digDirection = player.GetDirection();
			player.GetLevelPtr()->GetScene().Add(emptyTile);
		}

		auto coverTile{ player.GetGridPtr()->GetGrid()[index].coverTile };

		if (player.GetDirection() != player.GetGridPtr()->GetGrid()[index].digDirection) return;

		Point2f currentTilePos{ coverTile->GetComponent<dae::Transform>()->GetPosition().x, coverTile->GetComponent<dae::Transform>()->GetPosition().y };

		MoveDirection digDir = player.GetGridPtr()->GetGrid()[index].digDirection;

		if (!IsTileMoveAllowed(digDir, currentTilePos, playerPos)) return;

		coverTile->GetComponent<dae::Transform>()->SetPosition(playerPos.x, playerPos.y, 0.0f);

		player.GetGridPtr()->GetGrid()[index].hasBeenDug = CheckHasBeenDug(player, coverTile, index);

		if (player.GetGridPtr()->GetGrid()[index].hasBeenDug)
		{
			Point2f spawnPosition{ player.GetGridPtr()->GetGrid()[index].spawnPosition.x, player.GetGridPtr()->GetGrid()[index].spawnPosition.y };
			coverTile->GetComponent<dae::Transform>()->SetPosition(spawnPosition.x, spawnPosition.y, 0.0f);
		}
	}

	bool DiggingState::CheckHasBeenDug(Player& player, dae::GameObject* coverTile, int index)
	{
		Point2f tileLocation{ coverTile->GetComponent<dae::Transform>()->GetPosition().x, coverTile->GetComponent<dae::Transform>()->GetPosition().y };
		Point2f spawnPosition{ player.GetGridPtr()->GetGrid()[index].spawnPosition.x, player.GetGridPtr()->GetGrid()[index].spawnPosition.y };

		float deltaX = tileLocation.x - spawnPosition.x;
		float deltaY = tileLocation.y - spawnPosition.y;

		float distanceSquared = deltaX * deltaX + deltaY * deltaY;
		float margin = 3.f;

		if (distanceSquared <= margin * margin) return true;

		return false;
	}

	bool DiggingState::IsTileMoveAllowed(MoveDirection direction, const Point2f& currentTilePos, const Point2f& playerPos)
	{
		bool isMovingAllowed{ true };

		if (direction == MoveDirection::Left)
		{
			isMovingAllowed = playerPos.x < currentTilePos.x;
		}
		else if (direction == MoveDirection::Right)
		{
			isMovingAllowed = playerPos.x > currentTilePos.x;
		}
		else if (direction == MoveDirection::Up)
		{
			isMovingAllowed = playerPos.y < currentTilePos.y;
		}
		else if (direction == MoveDirection::Down)
		{
			isMovingAllowed = playerPos.y > currentTilePos.y;
		}

		return isMovingAllowed;
	}

	void DiggingState::OnEnter(Player& player)
	{
		m_Sprite = player.GetOwner()->GetComponent<SpriteComponent>();
		m_Sprite->SetNewTexture("Sprites/Player/DiggingSprite.png", 1, 16, 0, 1);

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
	
	PlayerStates::PlayerState* DeathState::Update(Player& player, float )
	{
		if (m_Sprite->HasReachedLastframe())
		{
			
		}

		switch (player.GetDirection())
		{
		case MoveDirection::Left:
			if (player.WasCrushed() && !m_Sprite->IsAlreadyWithinBounds(4, 5)) m_Sprite->SetSpriteBounds(4,   5, true);
			else if (!m_Sprite->IsAlreadyWithinBounds(16, 19))  				m_Sprite->SetSpriteBounds(16, 19, true);
			break;
		case MoveDirection::Right:
			if (player.WasCrushed() && !m_Sprite->IsAlreadyWithinBounds(0, 1)) m_Sprite->SetSpriteBounds(0, 1, true);
			else if (!m_Sprite->IsAlreadyWithinBounds(8, 11))  				m_Sprite->SetSpriteBounds(8, 11, true);
			break;
		case MoveDirection::Up:
			if (player.WasCrushed() && !m_Sprite->IsAlreadyWithinBounds(2, 3)) m_Sprite->SetSpriteBounds(2, 3, true);
			else if (!m_Sprite->IsAlreadyWithinBounds(12, 15))  				m_Sprite->SetSpriteBounds(12, 15, true);
			break;
		case MoveDirection::Down:
			if (player.WasCrushed() && !m_Sprite->IsAlreadyWithinBounds(6, 7)) m_Sprite->SetSpriteBounds(6, 7, true);
			else if (!m_Sprite->IsAlreadyWithinBounds(20, 23))  				m_Sprite->SetSpriteBounds(20, 23, true);
			break;
		}

		return nullptr;
	}
	
	void DeathState::OnEnter(Player& player)
	{
		m_Sprite = player.GetOwner()->GetComponent<SpriteComponent>();
		m_Sprite->SetNewTexture("Sprites/Player/DeathSprite.png", 3, 8, 0, 23);
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
		//Set throw sprite in direction

		// Get Weapon

		// If weapon does not hit enemy go back to idle

		// If weapon hits go to pumpsprite

		// if second pump takes too long go to idle

		// if second pump is on time, increase enemy inflate

		return nullptr;
	}

	void AttackState::SetPlayerThrow(Player& player) const
	{
		switch (player.GetDirection())
		{
		case MoveDirection::Left:
			m_Sprite->SetSpriteBounds(2, 2, true);
			break;
		case MoveDirection::Right:
			m_Sprite->SetSpriteBounds(0, 0, true);
			break;
		case MoveDirection::Up:
			m_Sprite->SetSpriteBounds(1, 1, true);
			break;
		case MoveDirection::Down:
			m_Sprite->SetSpriteBounds(3, 3, true);
			break;
		}
	}
	
	void AttackState::OnEnter(Player& player)
	{
		m_Sprite = player.GetOwner()->GetComponent<SpriteComponent>();
		SetPlayerThrow(player);
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
