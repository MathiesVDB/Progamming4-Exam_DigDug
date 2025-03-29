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
		if (auto transform = m_Owner->GetComponent<dae::Transform>())
		{
			auto pos = transform->GetPosition();
			switch (m_Direction)
			{
			case Direction::Left:
				transform->SetPosition(pos.x - 5.f, pos.y, pos.z);
				break;
			case Direction::Right:
				transform->SetPosition(pos.x + 5.f, pos.y, pos.z);
				break;
			case Direction::Up:
				transform->SetPosition(pos.x, pos.y - 5.f, pos.z);
				break;
			case Direction::Down:
				transform->SetPosition(pos.x, pos.y + 5.f, pos.z);
				break;
			}
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

//class PickupPelletCommand final : public Command
//{
//public:
//	explicit PickupPelletCommand(dae::GameObject* owner, ScoreDisplay* score)
//		: m_Owner(owner), m_Score(score)
//	{
//	}
//
//	void Execute() override
//	{
//		m_Score->SetScore(m_Score->GetScore() + 100);
//	}
//
//private:
//	dae::GameObject* m_Owner;
//	ScoreDisplay* m_Score;
//};