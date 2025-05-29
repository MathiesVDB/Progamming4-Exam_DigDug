#pragma once
#include "Transform.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "Player.h"

class Command
{
public:
	virtual ~Command() = default;
	virtual void Execute() = 0;
};

//------------------------------------------------
// Current Commands
//------------------------------------------------

class MoveCommand final : public Command
{
public:
	explicit MoveCommand(dae::GameObject* owner, MoveDirection direction, float movementSpeed, bool isEnemy = false)
		: m_Owner(owner), m_Direction(direction), m_Player{ nullptr }, m_IsEnemy{ isEnemy }, m_MovementSpeed{ movementSpeed }
	{
		if (!m_IsEnemy)
		{
			if (owner->HasComponent<Player>()) m_Player = owner->GetComponent<Player>();
			else std::cout << "'Player' component required to check gridmovement!\n";
		}
	}

	void Execute() override
	{
		//Enemy will always move straight to a target so checks don't need to happen 
		if (!m_IsEnemy)
		{
			if (!m_Player) return;
			if (m_Player->IsDead()) return;
			if (!m_Player->CanSwitchMovement(m_Direction)) return;
		}

		switch (m_Direction)
		{
		case MoveDirection::Left:
			m_Owner->SetVelocity({ -m_MovementSpeed, 0.f });
			break;
		case MoveDirection::Right:
			m_Owner->SetVelocity({ +m_MovementSpeed, 0.f });
			break;
		case MoveDirection::Up:
			m_Owner->SetVelocity({ 0.f, -m_MovementSpeed });
			break;
		case MoveDirection::Down:
			m_Owner->SetVelocity({ 0.f, +m_MovementSpeed });
			break;
		}
	}

private:
	dae::GameObject* m_Owner;
	MoveDirection m_Direction;
	Player* m_Player;

	float m_MovementSpeed{};

	bool m_IsEnemy{ false };
};

class AttackCommand final : public Command
{
public:
	explicit AttackCommand(dae::GameObject* owner)
		: m_Owner(owner)
	{
		if (owner->HasComponent<Player>()) m_Player = owner->GetComponent<Player>();
		else std::cout << "'Player' component required to attack!\n";
	}

	void Execute() override
	{
		m_Player->Attack();

		//if (auto health = m_Owner->GetComponent<HealthComponent>())
		//{
		//	health->TakeDamage(1);
		//}
	}

private:
	dae::GameObject* m_Owner;
	Player* m_Player{ nullptr };
};

