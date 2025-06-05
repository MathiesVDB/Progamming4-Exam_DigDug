#pragma once
#include "Transform.h"
#include "GameObject.h"
#include "Player.h"
#include "TextureComponent.h"
#include "SpriteComponent.h"

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
		if (m_Player->IsDead()) return;

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

class NextItemCommand : public Command
{
public:
	explicit NextItemCommand(std::vector<dae::GameObject*> items, bool isVertical)
		:   m_IsVertical{ isVertical },
			m_CurrentItemIndex{ 0 },
			m_Items{ items }
	{
		glm::vec2 currentItemPos{ m_Items[m_CurrentItemIndex]->GetWorldPosition() };
		glm::vec2 currentItemSize{};

		if (m_Items[m_CurrentItemIndex]->HasComponent<TextureComponent>())
		{
			currentItemSize.x = m_Items[m_CurrentItemIndex]->GetComponent<TextureComponent>()->GetWidth();
			currentItemSize.y = m_Items[m_CurrentItemIndex]->GetComponent<TextureComponent>()->GetHeight();
		}
		else if (m_Items[m_CurrentItemIndex]->HasComponent<SpriteComponent>())
		{
			currentItemSize = m_Items[m_CurrentItemIndex]->GetComponent<SpriteComponent>()->GetSpriteSize();
		}
		else
		{
			std::cerr << "ColliderComponent requires a TextureComponent or SpriteComponent to be attached to the same GameObject.\n";
			return;
		}

		if (m_IsVertical)
		{
			auto firstmarker = std::make_unique<dae::GameObject>();
			firstmarker->AddComponent<TextureComponent>("Sprites/Misc/Highlight/Arrow_Down.png");
			firstmarker->SetLocalPosition({ currentItemPos.y - 5, currentItemPos.x - currentItemSize.x / 2.f });
			firstmarker->SetRenderLayer(RenderLayer::Entity);

			m_FirstMarkerGameObject = firstmarker.get();

			auto lastmarker = std::make_unique<dae::GameObject>();
			lastmarker->AddComponent<TextureComponent>("Sprites/Misc/Highlight/Arrow_Up.png");
			lastmarker->SetLocalPosition({ currentItemPos.y + currentItemSize.y + 5, currentItemPos.x - currentItemSize.x / 2.f });
			lastmarker->SetRenderLayer(RenderLayer::Entity);

			m_LastMarkerGameObject = firstmarker.get();

			dae::SceneManager::GetInstance().GetActiveScene().Add(firstmarker);
			dae::SceneManager::GetInstance().GetActiveScene().Add(lastmarker);

			return;
		}

		auto firstmarker = std::make_unique<dae::GameObject>();
		firstmarker->AddComponent<TextureComponent>("Sprites/Misc/Highlight/Arrow_Right.png");
		firstmarker->SetLocalPosition({ currentItemPos.y - currentItemSize.y / 2.f, currentItemPos.x + currentItemSize.x + 5 });
		firstmarker->SetRenderLayer(RenderLayer::Entity);

		m_FirstMarkerGameObject = firstmarker.get();

		auto lastmarker = std::make_unique<dae::GameObject>();
		lastmarker->AddComponent<TextureComponent>("Sprites/Misc/Highlight/Arrow_Left.png");
		lastmarker->SetLocalPosition({ currentItemPos.y - currentItemSize.y / 2.f, currentItemPos.x + currentItemSize.x + 5 });
		lastmarker->SetRenderLayer(RenderLayer::Entity);

		m_LastMarkerGameObject = firstmarker.get();

		dae::SceneManager::GetInstance().GetActiveScene().Add(firstmarker);
		dae::SceneManager::GetInstance().GetActiveScene().Add(lastmarker);
	}

	void Execute() override
	{
		++m_CurrentItemIndex;

		SetNewMarkerPos();
	}

private:
	//private functions
	void SetNewMarkerPos()
	{
		glm::vec2 currentItemPos{ m_Items[m_CurrentItemIndex]->GetWorldPosition() };
		glm::vec2 currentItemSize{};

		if (m_Items[m_CurrentItemIndex]->HasComponent<TextureComponent>())
		{
			currentItemSize.x = m_Items[m_CurrentItemIndex]->GetComponent<TextureComponent>()->GetWidth();
			currentItemSize.y = m_Items[m_CurrentItemIndex]->GetComponent<TextureComponent>()->GetHeight();
		}
		else if (m_Items[m_CurrentItemIndex]->HasComponent<SpriteComponent>())
		{
			currentItemSize = m_Items[m_CurrentItemIndex]->GetComponent<SpriteComponent>()->GetSpriteSize();
		}
		else
		{
			std::cerr << "ColliderComponent requires a TextureComponent or SpriteComponent to be attached to the same GameObject.\n";
			return;
		}

		if (m_IsVertical)
		{
			m_FirstMarkerGameObject->SetLocalPosition({ currentItemPos.y - 5, currentItemPos.x - currentItemSize.x / 2.f });
			m_LastMarkerGameObject->SetLocalPosition({ currentItemPos.y + currentItemSize.y + 5, currentItemPos.x - currentItemSize.x / 2.f });

			return;
		}

		m_FirstMarkerGameObject->SetLocalPosition({ currentItemPos.y - currentItemSize.y / 2.f, currentItemPos.x + currentItemSize.x + 5 });
		m_LastMarkerGameObject->SetLocalPosition({ currentItemPos.y - currentItemSize.y / 2.f, currentItemPos.x + currentItemSize.x + 5 });
	}

	//member variables
	bool m_IsVertical;

	int m_CurrentItemIndex;

	dae::GameObject* m_FirstMarkerGameObject;
	dae::GameObject* m_LastMarkerGameObject;

	std::vector<dae::GameObject*> m_Items;
};