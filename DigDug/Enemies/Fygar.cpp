//---------------------------
// Include Files
//---------------------------
#include "Fygar.h"
#include "GridComponent.h"
#include "GameObject.h"
#include "Rock.h"
#include "SceneManager.h"
#include "Scene.h"

//---------------------------
// Constructor & Destructor
//---------------------------
Fygar::Fygar(dae::GameObject* owner, GridComponent* grid)
	:	Component(owner),
		m_GridPtr{ grid }
{
	m_FleeingTarget = m_GridPtr->GetGrid()[14].centerPoint;
	m_SpawnPosition = GetOwner()->GetWorldPosition();
	m_State = &FygarStates::FygarState::moving;
}

void Fygar::Update(float deltaTime)
{
	if (m_IsFleeing) m_CurrentTarget = m_FleeingTarget;
	else			 m_CurrentTarget = dae::SceneManager::GetInstance().GetActiveScene().GetPlayer(0)->GetWorldPosition();

	m_IsLookingLeft = m_CurrentTarget.x < GetOwner()->GetWorldPosition().x;

	auto newState = m_State->Update(*this, deltaTime);

	if (newState == nullptr) return;
	SetState(newState);
}

void Fygar::HandleCollision(const CollisionEvent& collision)
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
			SetState(&FygarStates::FygarState::dying);
		}
	}
}

void Fygar::SetState(FygarStates::FygarState* state)
{
	m_State->OnExit(*this);
	m_State = state;
	m_State->OnEnter(*this);
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
}

void Fygar::ResetInflation()
{
	m_InflatedState = Inflated::None;
}