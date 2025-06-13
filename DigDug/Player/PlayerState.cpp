#include "PlayerState.h"

#include "Fygar.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "HealthComponent.h"
#include "Level.h"
#include "Player.h"
#include "Pooka.h"
#include "RopeComponent.h"
#include "SpriteComponent.h"

namespace PlayerStates
{
	//-----------------------------------------------------
	// IdleState Class
	//-----------------------------------------------------
	
	void IdleState::Render(const Player& ) const
	{
	}
	
	std::unique_ptr<PlayerState> IdleState::Update(Player& player, float )
	{
		if (player.GetOwner()->GetVelocity() != glm::vec2{ 0, 0 }) return std::make_unique<MovingState>();

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
	
	std::unique_ptr<PlayerState> MovingState::Update(Player& player, float)
	{
		if (player.GetOwner()->GetVelocity() == glm::vec2{ 0, 0 }) return std::make_unique<IdleState>();

		glm::vec2 position{};
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

		if (!player.GetGridPtr()->GetGrid()[index].hasBeenDug && !player.GetGridPtr()->GetGrid()[index].hasRock) return std::make_unique<DiggingState>();
		
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
	
	std::unique_ptr<PlayerState> DiggingState::Update(Player& player, float )
	{
		if (player.GetOwner()->GetVelocity() == glm::vec2{ 0, 0}) return std::make_unique<IdleState>();

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

		if (player.GetGridPtr()->GetGrid()[index].hasBeenDug) return std::make_unique<MovingState>();

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

		glm::vec2 playerPos{ m_Position };
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
			dae::SceneManager::GetInstance().GetActiveScene().MarkForAdd(std::move(emptyTile));
		}

		auto coverTile{ player.GetGridPtr()->GetGrid()[index].coverTile };

		if (player.GetDirection() != player.GetGridPtr()->GetGrid()[index].digDirection) return;

		glm::vec2 currentTilePos{ coverTile->GetComponent<dae::Transform>()->GetPosition().x, coverTile->GetComponent<dae::Transform>()->GetPosition().y };

		MoveDirection digDir = player.GetGridPtr()->GetGrid()[index].digDirection;

		if (!IsTileMoveAllowed(digDir, currentTilePos, playerPos)) return;

		coverTile->GetComponent<dae::Transform>()->SetPosition(playerPos.x, playerPos.y);

		player.GetGridPtr()->GetGrid()[index].hasBeenDug = CheckHasBeenDug(player, coverTile, index);

		if (player.GetGridPtr()->GetGrid()[index].hasBeenDug)
		{
			auto originalTile = dae::SceneManager::GetInstance().GetActiveScene().GetObjectByPosition(player.GetGridPtr()->GetGrid()[index].spawnPosition);
			if (originalTile)
			{
				dae::SceneManager::GetInstance().GetActiveScene().MarkForDeletion(originalTile);
			}

			glm::vec2 spawnPosition{ player.GetGridPtr()->GetGrid()[index].spawnPosition.x, player.GetGridPtr()->GetGrid()[index].spawnPosition.y };
			coverTile->GetComponent<dae::Transform>()->SetPosition(spawnPosition.x, spawnPosition.y);
		}
	}

	bool DiggingState::CheckHasBeenDug(Player& player, dae::GameObject* coverTile, int index)
	{
		glm::vec2 tileLocation{ coverTile->GetComponent<dae::Transform>()->GetPosition().x, coverTile->GetComponent<dae::Transform>()->GetPosition().y };
		glm::vec2 spawnPosition{ player.GetGridPtr()->GetGrid()[index].spawnPosition.x, player.GetGridPtr()->GetGrid()[index].spawnPosition.y };

		float deltaX = tileLocation.x - spawnPosition.x;
		float deltaY = tileLocation.y - spawnPosition.y;

		float distanceSquared = deltaX * deltaX + deltaY * deltaY;
		float margin = 3.f;

		if (distanceSquared <= margin * margin) return true;

		return false;
	}

	bool DiggingState::IsTileMoveAllowed(MoveDirection direction, const glm::vec2& currentTilePos, const glm::vec2& playerPos)
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
	
	std::unique_ptr<PlayerState> DeathState::Update(Player& player, float )
	{
		if (m_Sprite->HasReachedLastframe())
		{
			player.GetHealth()->TakeDamage();

			//Don't reset in two player mode
			if (dae::SceneManager::GetInstance().GetActiveScene().GetObjectsByTag("Player").size() <= 1)
			{
				auto entities{ dae::SceneManager::GetInstance().GetActiveScene().GetObjectsByTag("Enemy") };

				for (const auto& entity : entities)
				{
					if (entity->HasComponent<Pooka>()) entity->GetComponent<Pooka>()->ResetPooka();
					else if (entity->HasComponent<Fygar>()) entity->GetComponent<Fygar>()->ResetFygar();
				}

				player.ResetPlayer();
			}

			player.StartInvincibiltyPeriod();

			return std::make_unique<IdleState>();
		}

		return nullptr;
	}
	
	void DeathState::OnEnter(Player& player)
	{
		m_Sprite = player.GetOwner()->GetComponent<SpriteComponent>();
		m_Sprite->SetNewTexture("Sprites/Player/DeathSprite.png", 3, 8, 0, 23);

		switch (player.GetDirection())
		{
		case MoveDirection::Left:
			if (player.WasCrushed()) m_Sprite->SetSpriteBounds(4, 5, true);
			else 					 m_Sprite->SetSpriteBounds(16, 19, true);
			break;
		case MoveDirection::Right:
			if (player.WasCrushed()) m_Sprite->SetSpriteBounds(0, 1, true);
			else 					 m_Sprite->SetSpriteBounds(8, 11, true);
			break;
		case MoveDirection::Up:
			if (player.WasCrushed()) m_Sprite->SetSpriteBounds(2, 3, true);
			else					 m_Sprite->SetSpriteBounds(12, 15, true);
			break;
		case MoveDirection::Down:
			if (player.WasCrushed()) m_Sprite->SetSpriteBounds(6, 7, true);
			else  					 m_Sprite->SetSpriteBounds(20, 23, true);
			break;
		}
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
	
	std::unique_ptr<PlayerState> AttackState::Update(Player& player, float deltaTime)
	{
		if (player.GetOwner()->GetVelocity() != glm::vec2{ 0, 0 }) return std::make_unique<MovingState>();

		if (!player.GetRopePtr()->GetRopeStatus()) return std::make_unique<MovingState>();

		if (player.GetRopePtr()->GetHasHit() && !m_InitializedPump) m_IsPumping = true;
		if (m_IsPumping) SetPlayerPump(player);

		if (player.GetRopePtr()->IsAttacking()) m_Sprite->SetSpriteBounds(++m_StartPumpFrame, ++m_StartPumpFrame, true);
		else									m_Sprite->SetSpriteBounds(  m_StartPumpFrame  , m_StartPumpFrame, true);

		player.GetRopePtr()->Update(deltaTime);

		return nullptr;
	}

	void AttackState::SetPlayerThrow(Player& player) const
	{
		m_Sprite->SetNewTexture("Sprites/Player/ThrowDarkSprite.png", 1, 4, 0, 0);

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

	void AttackState::SetPlayerPump(Player& player)
	{
		m_Sprite->SetNewTexture("Sprites/Player/PumpSprite.png", 1, 16, 0, 0);

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
		m_StartPumpFrame = m_Sprite->GetCurrentFrame();
		m_InitializedPump = true;
	}
	
	void AttackState::OnEnter(Player& player)
	{
		m_Sprite = player.GetOwner()->GetComponent<SpriteComponent>();
		SetPlayerThrow(player);

		glm::vec2 ropeStart{
			player.GetOwner()->GetWorldPosition().x,
			player.GetOwner()->GetWorldPosition().y + player.GetCollider()->GetBoundingBox().h / 3.f };

		player.GetRopePtr()->ActivateRope(ropeStart, player.GetDirection());

		player.NotifyAttack();
	}
	
	void AttackState::OnExit(Player& player)
	{
		player.GetRopePtr()->ResetRope();
	}
}
