#include "PlayerState.h"

#include "GameObject.h"
#include "GridComponent.h"
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
		if (player.GetOwner()->GetVelocity() == glm::vec3{ 0, 0, 0 })
		{
			return &PlayerStates::PlayerState::idling;
		}

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

		if (player.GetGridPtr()->GetGrid()[player.GetGridPtr()->GetCellIndex(player.)])

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
	
	void DiggingState::OnEnter(Player& player)
	{
		player.GetOwner()->GetComponent<SpriteComponent>()->SetNewTexture("Sprites/Player/DiggingSprite.png", 1, 16, 0, 1);
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
