#include "RopeComponent.h"
#include "ColliderComponent.h"
#include "Fygar.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "Helpers.h"
#include "Pooka.h"
#include "RealCollisionSystem.h"

RopeComponent::RopeComponent(dae::GameObject* owner, dae::GameObject* head, dae::GameObject* middle, dae::GameObject* tail)
	:	Component(owner),
		m_RopeHead	{ head   },
		m_RopeMiddle{ middle },
		m_RopeTail	{ tail   },
		m_StartPos	{ 0, 0 },
		m_Active	{ false },
		m_IsAttacking{false},
		m_ElapsedDistance{}
{
	m_RopeCollider = m_RopeHead->GetComponent<ColliderComponent>();

	//All parts of the rope are the same width
	m_Width = static_cast<float>(m_RopeHead->GetComponent<TextureComponent>()->GetWidth());
	// I had to either cast here of in update -> glm::vec2 cannot multiply with int but can with float
}

void RopeComponent::Update(float deltaTime)
{
	if (!m_Active)
		return;

	if (m_HasHit)
	{
		if (m_IsAttacking)
		{
			if (m_HitTarget->HasComponent<Pooka>())
				m_HitTarget->GetComponent<Pooka>()->IncreaseInflation();
			else if (m_HitTarget->HasComponent<Fygar>())
				m_HitTarget->GetComponent<Fygar>()->IncreaseInflation();
			
			ToggleAttacking();
		}
		return;
	}

	const float movement = ROPE_SPEED * deltaTime;
	m_ElapsedDistance += movement;

	glm::vec2 direction{};
	switch (m_Direction)
	{
	case MoveDirection::Up:    direction = {  0, -1 }; break;
	case MoveDirection::Down:  direction = {  0,  1 }; break;
	case MoveDirection::Left:  direction = { -1,  0 }; break;
	case MoveDirection::Right: direction = {  1,  0 }; break;
	}

	// Move head
	m_RopeHead->SetLocalPosition(m_StartPos + direction * m_ElapsedDistance);

	// Move middle
	if (m_ElapsedDistance >= m_Width)
	{
		float middleDistance = m_ElapsedDistance - m_Width;
		m_RopeMiddle->SetLocalPosition(m_StartPos + direction * middleDistance);
	}

	// Move tail
	if (m_ElapsedDistance >= m_Width * 2)
	{
		float tailDistance = m_ElapsedDistance - m_Width * 2;
		m_RopeTail->SetLocalPosition(m_StartPos + direction * tailDistance);
	}

	// Move rope out of screen when at max distance
	if (m_ElapsedDistance >= m_Width * 3)
	{
		m_Active = false;

		const glm::vec2 offscreenPos{ -100.f, -100.f };

		auto moveOffscreenIfNeeded = [&](dae::GameObject* segment)
			{
				if (segment->GetWorldPosition() != offscreenPos)
				{
					segment->SetLocalPosition(offscreenPos);
				}
			};

		moveOffscreenIfNeeded(m_RopeHead);
		moveOffscreenIfNeeded(m_RopeMiddle);
		moveOffscreenIfNeeded(m_RopeTail);
	}
}

void RopeComponent::HandleCollision(const CollisionEvent& collision)
{
	if (m_HasHit || !m_Active)
		return;

	const auto& colliderTag = collision.collider->GetComponent<ColliderComponent>()->GetTag();
	const auto& collidedTag = collision.collided->GetComponent<ColliderComponent>()->GetTag();

	bool colliderIsEnemy = colliderTag == Tag::ENEMY_ENTITY;
	bool collidedIsEnemy = collidedTag == Tag::ENEMY_ENTITY;

	if (colliderIsEnemy || collidedIsEnemy)
	{
		m_HasHit = true;

		if (colliderIsEnemy)
			m_HitTarget = collision.collider;
		else if (collidedIsEnemy)
			m_HitTarget = collision.collided;
	}
}

void RopeComponent::ActivateRope(const glm::vec2& startPos, MoveDirection shootDirection)
{
	m_Direction = shootDirection;
	m_StartPos	= startPos;
	m_ElapsedDistance = 0.0f;
	m_Active = true;
	m_HasHit = false;
	m_HitTarget = nullptr;

	m_RopeHead->SetLocalPosition(m_StartPos);

	ChangeRopeTexture(m_Direction);
}

void RopeComponent::ChangeRopeTexture(MoveDirection shootDirection) const
{
	auto headTexture	= m_RopeHead->GetComponent<TextureComponent>();
	auto middleTexture	= m_RopeMiddle->GetComponent<TextureComponent>();
	auto tailTexture	= m_RopeTail->GetComponent<TextureComponent>();

	std::string defaultPath{ "Sprites/Player/Weapon" };

	switch (shootDirection)
	{
	case MoveDirection::Down:
		headTexture->SetTexture(defaultPath + "/Down/DownHead.png");
		middleTexture->SetTexture(defaultPath + "/Down/DownMiddle.png");
		tailTexture->SetTexture(defaultPath + "/Down/DownTail.png");
		break;
	case MoveDirection::Left:
		headTexture->SetTexture(defaultPath + "/Left/LeftHead.png");
		middleTexture->SetTexture(defaultPath + "/Left/LeftMiddle.png");
		tailTexture->SetTexture(defaultPath + "/Left/LeftTail.png");
		break;
	case MoveDirection::Right:
		headTexture->SetTexture(defaultPath + "/Right/RightHead.png");
		middleTexture->SetTexture(defaultPath + "/Right/RightMiddle.png");
		tailTexture->SetTexture(defaultPath + "/Right/RightTail.png");
		break;
	case MoveDirection::Up:
		headTexture->SetTexture(defaultPath + "/Up/UpHead.png");
		middleTexture->SetTexture(defaultPath + "/Up/UpMiddle.png");
		tailTexture->SetTexture(defaultPath + "/Up/UpTail.png");
		break;
	}
}