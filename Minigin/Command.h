#pragma once
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "Observer.h"
#include "HealthComponent.h"

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
	enum class Direction
	{
		Left,
		Right,
		Up,
		Down
	};

	explicit MoveCommand(dae::GameObject* owner, Direction direction)
		: m_Owner(owner), m_Direction(direction) {
	}

	void Execute() override
	{
		switch (m_Direction)
		{
		case Direction::Left:
			m_Owner->SetVelocity({ -50.f, 0.f, 0.f });
			break;
		case Direction::Right:
			m_Owner->SetVelocity({ +50.f, 0.f, 0.f });
			break;
		case Direction::Up:
			m_Owner->SetVelocity({ 0.f, -50.f, 0.f });
			break;
		case Direction::Down:
			m_Owner->SetVelocity({ 0.f, +50.f, 0.f });
			break;
		}
	}

private:
	dae::GameObject* m_Owner;
	Direction m_Direction;
};

class DamageCommand final : public Command
{
public:
	explicit DamageCommand(dae::GameObject* owner)
		: m_Owner(owner) {
	}

	void Execute() override
	{
		if (auto health = m_Owner->GetComponent<HealthComponent>())
		{
			health->TakeDamage(1);
		}
	}

private:
	dae::GameObject* m_Owner;
};

