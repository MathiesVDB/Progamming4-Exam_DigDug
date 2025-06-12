#include "Pooka.h"
#include "SpriteComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Rock.h"
#include "RopeComponent.h"
#include "RopeHeadComponent.h"
#include "Scene.h"
#include "SceneManager.h"

Pooka::Pooka(dae::GameObject* owner, GridComponent* grid)
	:	Component(owner),
		m_GridPtr{ grid },
		m_State{ std::make_unique<PookaStates::MovingState>() }
{
	m_FleeingTarget = m_GridPtr->GetGrid()[14].centerPoint;
	m_SpawnPosition = GetOwner()->GetWorldPosition();
	SetState(std::make_unique<PookaStates::MovingState>());
}

void Pooka::Update(float deltaTime)
{
	if (m_IsFleeing) 
		m_CurrentTarget = m_FleeingTarget;
	else
	{
		const auto& players = dae::SceneManager::GetInstance().GetActiveScene().GetObjectsByTag("Player");
		assert(!players.empty() && "NO PLAYERS FOUND");

		const glm::vec2 myPos = GetOwner()->GetWorldPosition();

		float closestDistance = std::numeric_limits<float>::max();
		glm::vec2 closestTarget{};

		for (const auto& player : players)
		{
			const glm::vec2 playerPos = player->GetWorldPosition();
			const float distance = glm::distance(myPos, playerPos);

			if (distance < closestDistance)
			{
				closestDistance = distance;
				closestTarget = playerPos;
			}
		}

		m_CurrentTarget = closestTarget;
	}

	m_IsLookingLeft = m_CurrentTarget.x < GetOwner()->GetWorldPosition().x;
	
	if (auto newState = m_State->Update(*this, deltaTime))
	{
		SetState(std::move(newState));
	}
}

void Pooka::Render() const
{
	m_State->Render(*this);
}

void Pooka::HandleCollision(const CollisionEvent& collision)
{
	if (dynamic_cast<PookaStates::GhostState*>(m_State.get()) != nullptr) return;

	const auto& colliderTag{ collision.collider->GetComponent<ColliderComponent>()->GetTag() };
	const auto& collidedTag{ collision.collided->GetComponent<ColliderComponent>()->GetTag() };

	if (colliderTag == Tag::ROCK || collidedTag == Tag::ROCK)
	{
		auto rockObject = (colliderTag == Tag::ROCK) ? collision.collider : collision.collided;
		auto rock = rockObject->GetComponent<Rock>();

		if (rock->IsFalling()) GetOwner()->SetLocalPosition({ rockObject->GetLocalPosition().x, rockObject->GetLocalPosition().y + 30 });
		else if (rock->IsBreaking())
		{
			m_WasCrushed = true;
			SetState(std::make_unique<PookaStates::DeathState>());
		}
	}
	else if (collidedTag == ROPE || colliderTag == ROPE)
	{
		if (m_InflatedState == Inflated::Exploded || dynamic_cast<PookaStates::DeathState*>(m_State.get()) != nullptr) return;

		RopeComponent* rope = GetRopeFromCollision(collision);

		if (dynamic_cast<PookaStates::InflatedState*>(m_State.get()) == nullptr || m_IsDeflating)
		{
			SetState(std::make_unique<PookaStates::InflatedState>());
			rope->SetHasHit();
			m_IsDeflating = false;
		}

		if (rope && rope->IsAttacking())
		{
			IncreaseInflation();
			rope->ToggleAttacking();
			rope->SetHasHit();
		}
	}

}

RopeComponent* Pooka::GetRopeFromCollision(const CollisionEvent& collision)
{
	dae::GameObject* ropeObj = nullptr;

	if (collision.collided->HasComponent<RopeHeadComponent>())
		ropeObj = collision.collided;
	else if (collision.collider->HasComponent<RopeHeadComponent>())
		ropeObj = collision.collider;

	if (ropeObj)
		return ropeObj->GetComponent<RopeHeadComponent>()->GetRopeOwner();

	return nullptr;
}


void Pooka::SetState(std::unique_ptr<PookaStates::PookaState> newState)
{
	m_State->OnExit(*this);
	m_State = std::move(newState);
	m_State->OnEnter(*this);
}

void Pooka::ResetPooka()
{
	GetOwner()->SetLocalPosition(m_SpawnPosition);
	SetState(std::make_unique<PookaStates::MovingState>());
}

void Pooka::IncreaseInflation()
{
	if (m_InflatedState == Inflated::Exploded) return;

	switch (m_InflatedState)
	{
	case Inflated::None:
		m_InflatedState = Inflated::Stage1;
		break;
	case Inflated::Stage1:
		m_InflatedState = Inflated::Stage2;
		break;
	case Inflated::Stage2:
		m_InflatedState = Inflated::Stage3;
		break;
	case Inflated::Stage3:
		m_InflatedState = Inflated::Exploded;
		break;
	default:
		break;
	}

	dae::EventID PumpEnemyEventID = dae::EventRegistry::GetInstance().GetEventID("PumpEnemy");
	Notify(GetOwner(), PumpEnemyEventID);
}

void Pooka::DecreaseInflation()
{
	m_IsDeflating = true;

	if (m_InflatedState == Inflated::Exploded) return;

	switch (m_InflatedState)
	{
	case Inflated::None:
		SetState(std::make_unique<PookaStates::MovingState>());
		m_IsDeflating = false;
		break;
	case Inflated::Stage1:
		m_InflatedState = Inflated::None;
		break;
	case Inflated::Stage2:
		m_InflatedState = Inflated::Stage1;
		break;
	case Inflated::Stage3:
		m_InflatedState = Inflated::Stage2;
		break;
	default:
		break;
	}
}

void Pooka::NotifyDeath() const
{
	dae::EventID EnemyDiedEventID = dae::EventRegistry::GetInstance().GetEventID("EnemyDied");

	Notify(GetOwner(), EnemyDiedEventID);
}