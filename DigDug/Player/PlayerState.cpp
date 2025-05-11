#include "PlayerState.h"

#include "GameObject.h"
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
	
	void IdleState::Update(Player& player, float )
	{
		if (player.GetOwner()->GetVelocity() != glm::vec3{0, 0, 0}) player.SetState(&PlayerStates::PlayerState::moving);
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
	
	void MovingState::Update(Player& player, float)
	{
		if (player.GetOwner()->GetVelocity() == glm::vec3{ 0, 0, 0 })
		{
			player.SetState(&PlayerStates::PlayerState::idling);
			return;
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
	
	void DiggingState::Update(Player& , float )
	{
	}
	
	void DiggingState::OnEnter(Player& )
	{
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
	
	void DeathState::Update(Player& , float )
	{
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
	
	void AttackState::Update(Player& , float )
	{
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
