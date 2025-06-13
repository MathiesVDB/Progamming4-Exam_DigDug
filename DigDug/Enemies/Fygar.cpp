//---------------------------
// Include Files
//---------------------------
#include "Fygar.h"
#include "FygarState.h"
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
Fygar::Fygar(dae::GameObject* owner, GridComponent* grid, bool isControlled)
	:	Component(owner),
		m_GridPtr{ grid },
		m_State{std::make_unique<FygarStates::MovingState>()},
		m_IsControlled{ isControlled },
		m_Direction{ MoveDirection::Left }
{
	m_FleeingTarget = m_GridPtr->GetGrid()[14].centerPoint;
	m_SpawnPosition = GetOwner()->GetWorldPosition();
	SetState(std::make_unique<FygarStates::MovingState>());
	SnapToCellCenter();
}
void Fygar::SnapToCellCenter()
{
	if (!m_GridPtr || !GetOwner()->HasComponent<ColliderComponent>())
		return;

	auto boundingBox = GetOwner()->GetComponent<ColliderComponent>()->GetBoundingBox();

	glm::vec2 center = {
		boundingBox.x + boundingBox.w / 2.0f,
		boundingBox.y + boundingBox.h / 2.0f
	};

	int index = m_GridPtr->GetCellIndex(center);
	if (index == -1) return;

	auto cellCenter = m_GridPtr->GetGrid()[index].centerPoint;

	float newX = cellCenter.x - boundingBox.w / 2.0f;
	float newY = cellCenter.y - boundingBox.h / 2.0f;

	GetOwner()->GetComponent<dae::Transform>()->SetPosition(newX, newY);
}

bool Fygar::CanMove() const
{
	bool isMoving	{ dynamic_cast<FygarStates::MovingState*>(m_State.get()) != nullptr };
	bool isGhosting	{ dynamic_cast<FygarStates::GhostState*>(m_State.get()) != nullptr };

	return isMoving || isGhosting;
}

bool Fygar::CanSwitchMovement(MoveDirection direction)
{
	//These movement functions are for keeping character from moving wrongly over the grid. AI will never do so, so just return true
	if (!m_IsControlled) return true; 

	const auto boundingBox = GetOwner()->GetComponent<ColliderComponent>()->GetBoundingBox();

	if (m_Direction == direction) return true;

	if ((IsVertical(m_Direction) && IsHorizontal(direction)) || (IsHorizontal(m_Direction) && IsVertical(direction)))
	{
		bool canSwitch = false;
		if (IsVertical(direction))
			canSwitch = CanMoveVertical(boundingBox);
		else
			canSwitch = CanMoveHorizontal(boundingBox);

		if (!canSwitch)
			return false;
	}

	m_Direction = direction;
	return true;
}

bool Fygar::CanMoveHorizontal(const SDL_Rect& boundingBox)
{
	glm::vec2 center = {
		boundingBox.x + boundingBox.w / 2.0f,
		boundingBox.y + boundingBox.h / 2.0f
	};

	int index = m_GridPtr->GetCellIndex(center);
	if (index == -1) return false;

	auto cellCenter = m_GridPtr->GetGrid()[index].centerPoint;

	float distanceToCenter = center.y - cellCenter.y;

	if (std::abs(distanceToCenter) <= SNAP_DISTANCE)
	{
		SnapToCellCenter();
		return true;
	}

	float correction = 1 * (distanceToCenter > 0 ? -1.f : 1.f); // move up/down
	auto currentPos = GetOwner()->GetComponent<dae::Transform>()->GetPosition();
	GetOwner()->GetComponent<dae::Transform>()->SetPosition(currentPos.x, currentPos.y + correction);

	return false;
}

bool Fygar::CanMoveVertical(const SDL_Rect& boundingBox)
{
	glm::vec2 center = {
		boundingBox.x + boundingBox.w / 2.0f,
		boundingBox.y + boundingBox.h / 2.0f
	};

	int index = m_GridPtr->GetCellIndex(center);
	if (index < 0 || index >= static_cast<int>(m_GridPtr->GetGrid().size())) return false;

	auto cellCenter = m_GridPtr->GetGrid()[index].centerPoint;

	float distanceToCenter = center.x - cellCenter.x;

	if (std::abs(distanceToCenter) <= SNAP_DISTANCE)
	{
		SnapToCellCenter();
		return true;
	}

	float correction = 2 * (distanceToCenter > 0 ? -1.f : 1.f); // move left/right
	auto currentPos = GetOwner()->GetComponent<dae::Transform>()->GetPosition();
	GetOwner()->GetComponent<dae::Transform>()->SetPosition(currentPos.x + correction, currentPos.y);

	return false;
}

bool Fygar::IsVertical(MoveDirection dir) const
{
	return dir == MoveDirection::Up || dir == MoveDirection::Down;
}

bool Fygar::IsHorizontal(MoveDirection dir) const
{
	return dir == MoveDirection::Left || dir == MoveDirection::Right;
}

void Fygar::Update(float deltaTime)
{
	if (m_IsFleeing) m_CurrentTarget = m_FleeingTarget;
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

void Fygar::Attack()
{
	//Fygar can only attack when moving 
	if (dynamic_cast<FygarStates::MovingState*>(m_State.get()) == nullptr) return;

	SetState(std::make_unique<FygarStates::AttackState>());
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

		if (rock->IsFalling())
		{
			if (!m_HasRegisteredCrush)
			{
				rock->IncrementCrushCount();
				m_HasRegisteredCrush = true;
			}
			GetOwner()->SetLocalPosition({ rockObject->GetLocalPosition().x, rockObject->GetLocalPosition().y + 30 });
		}
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

		if (dynamic_cast<FygarStates::InflatedState*>(m_State.get()) == nullptr || m_IsDeflating)
		{
			SetState(std::make_unique<FygarStates::InflatedState>());
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

void Fygar::ResetFygar()
{
	GetOwner()->SetLocalPosition(m_SpawnPosition);
	SetState(std::make_unique<FygarStates::MovingState>());
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

void Fygar::DecreaseInflation()
{
	m_IsDeflating = true;

	if (m_InflatedState == Inflated::Exploded) return;

	switch (m_InflatedState)
	{
	case Inflated::None:
		SetState(std::make_unique<FygarStates::MovingState>());
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

bool Fygar::IsDeadly() const
{
	bool isMoving	{ dynamic_cast<FygarStates::MovingState*>(m_State.get()) != nullptr };
	bool isAttacking{ dynamic_cast<FygarStates::AttackState*>(m_State.get()) != nullptr };

	return isMoving || isAttacking;
}
