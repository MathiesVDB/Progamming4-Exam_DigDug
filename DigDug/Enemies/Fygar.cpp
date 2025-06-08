//---------------------------
// Include Files
//---------------------------
#include "Fygar.h"
#include "GridComponent.h"
#include "GameObject.h"
#include "PookaState.h"
#include "Rock.h"
#include "RopeHeadComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "RopeComponent.h"

//---------------------------
// Constructor & Destructor
//---------------------------
Fygar::Fygar(dae::GameObject* owner, GridComponent* grid)
	:	Component(owner),
		m_GridPtr{ grid },
		m_State{std::make_unique<FygarStates::MovingState>()}
{
	m_FleeingTarget = m_GridPtr->GetGrid()[14].centerPoint;
	m_SpawnPosition = GetOwner()->GetWorldPosition();
	SetState(std::make_unique<FygarStates::MovingState>());
}

void Fygar::Update(float deltaTime)
{
	if (m_IsFleeing) m_CurrentTarget = m_FleeingTarget;
	else
	{
		const auto& players = dae::SceneManager::GetInstance().GetActiveScene().GetObjectsByTag("Player");

		assert(!players.empty() && "NO PLAYERS FOUND");

		m_CurrentTarget = players[0]->GetWorldPosition();
	}

	m_IsLookingLeft = m_CurrentTarget.x < GetOwner()->GetWorldPosition().x;

	if (auto newState = m_State->Update(*this, deltaTime))
	{
		SetState(std::move(newState));
	}
}

void Fygar::HandleCollision(const CollisionEvent& collision)
{
	if (dynamic_cast<FygarStates::GhostState*>(m_State.get()) != nullptr) return;

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
			SetState(std::make_unique<FygarStates::DeathState>());
		}
	}
	else if (collidedTag == ROPE || colliderTag == ROPE)
	{
		if (m_InflatedState == Inflated::Exploded || dynamic_cast<FygarStates::DeathState*>(m_State.get()) != nullptr) return;

		RopeComponent* rope = GetRopeFromCollision(collision);

		if (dynamic_cast<FygarStates::InflatedState*>(m_State.get()) == nullptr)
		{
			SetState(std::make_unique<FygarStates::InflatedState>());
			rope->SetHasHit();
		}

		if (rope && rope->IsAttacking())
		{
			IncreaseInflation();
			rope->ToggleAttacking();
			rope->SetHasHit();
		}
	}
}

RopeComponent* Fygar::GetRopeFromCollision(const CollisionEvent& collision)
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

void Fygar::SetState(std::unique_ptr<FygarStates::FygarState> newState)
{
	m_State->OnExit(*this);
	m_State = std::move(newState);
	m_State->OnEnter(*this);
}

void Fygar::NotifyDeath() const
{
	dae::EventID EnemyDiedEventID = dae::EventRegistry::GetInstance().GetEventID("EnemyDied");

	Notify(GetOwner(), EnemyDiedEventID);
}

void Fygar::NotifyAttack() const
{
	dae::EventID FygarFireEventID = dae::EventRegistry::GetInstance().GetEventID("FygarFire");

	Notify(GetOwner(), FygarFireEventID);
}

void Fygar::IncreaseInflation()
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

void Fygar::ResetInflation()
{
	m_InflatedState = Inflated::None;
}

bool Fygar::IsDeadly() const
{
	bool isMoving	{ dynamic_cast<FygarStates::MovingState*>(m_State.get()) != nullptr };
	bool isAttacking{ dynamic_cast<FygarStates::AttackState*>(m_State.get()) != nullptr };

	return isMoving || isAttacking;
}
