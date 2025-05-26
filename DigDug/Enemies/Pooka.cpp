#include "Pooka.h"

#include "SpriteComponent.h"
#include "GameObject.h"
#include "Rock.h"
#include "Scene.h"
#include "SceneManager.h"

Pooka::Pooka(dae::GameObject* owner, GridComponent* grid)
	:	Component(owner),
		m_GridPtr{ grid },
		m_State{ &PookaStates::PookaState::moving }
{
	m_FleeingTarget = m_GridPtr->GetGrid()[14].centerPoint;
	m_SpawnPosition = GetOwner()->GetWorldPosition();
	SetState(&PookaStates::PookaState::moving);
}

void Pooka::Update(float deltaTime)
{
	if (m_IsFleeing) m_CurrentTarget = m_FleeingTarget;
	else			 m_CurrentTarget = dae::SceneManager::GetInstance().GetActiveScene().GetPlayer(0)->GetWorldPosition();

	m_IsLookingLeft = m_CurrentTarget.x < GetOwner()->GetWorldPosition().x;

	auto newState = m_State->Update(*this, deltaTime);

	if (newState == nullptr) return;
	SetState(newState);
}

void Pooka::Render() const
{
	
}

void Pooka::HandleCollision(const CollisionEvent& collision)
{
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
			SetState(&PookaStates::PookaState::dying);
		}
	}
}

void Pooka::SetState(PookaStates::PookaState* state)
{
	m_State->OnExit(*this);
	m_State = state;
	m_State->OnEnter(*this);
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
}

void Pooka::ResetInflation()
{
	m_InflatedState = Inflated::None;
}
