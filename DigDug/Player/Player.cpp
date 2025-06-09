//---------------------------
// Include Files
//---------------------------
#include "Player.h"
#include "RopeComponent.h"
#include "Fygar.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "Pooka.h"
#include "Rock.h"
//---------------------------
// Constructor & Destructor
//---------------------------
Player::Player(dae::GameObject* owner, GridComponent* grid)
	:	Component(owner),
		m_GridPtr{ grid },
		m_Direction{ MoveDirection::Right },
		m_State{std::make_unique<PlayerStates::IdleState>()}
{
	m_Collider  = GetOwner()->GetComponent<ColliderComponent>();
    m_Rope      = GetOwner()->GetComponent<RopeComponent>();
    m_Health    = GetOwner()->GetComponent<HealthComponent>();

    SetState(std::make_unique<PlayerStates::IdleState>());

    SnapToCellCenter();

    m_SpawnPosition = GetOwner()->GetWorldPosition();
}

void Player::Update(float deltaTime)
{
    if (m_IsInvincible)
    {
        m_AccumulatedTime += deltaTime;

        if (m_AccumulatedTime >= INVINCIBILITY_TIME)
        {
            m_AccumulatedTime = 0;
            m_IsInvincible = false;
        }
    }

    if (auto newState = m_State->Update(*this, deltaTime))
    {
        SetState(std::move(newState));
    }
}

void Player::Render() const
{
    m_State->Render(*this);
}

void Player::Attack()
{
    if (dynamic_cast<PlayerStates::AttackState*>(m_State.get()) == nullptr)
    {
        SetState(std::make_unique<PlayerStates::AttackState>());
        return;
    }

    m_Rope->ToggleAttacking();
}

void Player::SetState(std::unique_ptr<PlayerStates::PlayerState> state)
{
    m_State->OnExit(*this);
    m_State = std::move(state);
    m_State->OnEnter(*this);
}

void Player::SnapToCellCenter()
{
    if (!m_GridPtr || !GetOwner()->HasComponent<ColliderComponent>())
        return;

    auto boundingBox = GetOwner()->GetComponent<ColliderComponent>()->GetBoundingBox();

    glm::vec2 center = {
        boundingBox.x + boundingBox.w / 2.0f,
        boundingBox.y + boundingBox.h / 2.0f
    };

    int index = m_GridPtr->GetCellIndex(center);
    auto cellCenter = m_GridPtr->GetGrid()[index].centerPoint;

    float newX = cellCenter.x - boundingBox.w / 2.0f;
    float newY = cellCenter.y - boundingBox.h / 2.0f;

    GetOwner()->GetComponent<dae::Transform>()->SetPosition(newX, newY);
}

void Player::NotifyAttack() const
{
    dae::EventID PlayerAttackEventID = dae::EventRegistry::GetInstance().GetEventID("PlayerAttack");

    Notify(GetOwner(), PlayerAttackEventID);
}

void Player::HandleCollision(const CollisionEvent& collision)
{
    if (dynamic_cast<PlayerStates::AttackState*>(m_State.get()) != nullptr) m_Rope->HandleCollision(collision);
    if (dynamic_cast<PlayerStates::DeathState*>(m_State.get()) != nullptr) return;
    if (m_IsInvincible) return;

    const auto& colliderTag{ collision.collider->GetComponent<ColliderComponent>()->GetTag() };
    const auto& collidedTag{ collision.collided->GetComponent<ColliderComponent>()->GetTag() };

	if (colliderTag == Tag::ENEMY_ENTITY || collidedTag == Tag::ENEMY_ENTITY)
	{
        bool isPlayerDead{ true };

        if      (collision.collider->HasComponent<Pooka>()) isPlayerDead = collision.collider->GetComponent<Pooka>()->IsDeadly();
        else if (collision.collided->HasComponent<Pooka>()) isPlayerDead = collision.collided->GetComponent<Pooka>()->IsDeadly();
        else if (collision.collider->HasComponent<Fygar>()) isPlayerDead = collision.collider->GetComponent<Fygar>()->IsDeadly();
        else if (collision.collided->HasComponent<Fygar>()) isPlayerDead = collision.collided->GetComponent<Fygar>()->IsDeadly();

        if (!isPlayerDead) return;

        m_WasCrushed = false;
        SetState(std::make_unique<PlayerStates::DeathState>());

        dae::EventID PlayerHitEventID = dae::EventRegistry::GetInstance().GetEventID("PlayerHit");
        Notify(GetOwner(), PlayerHitEventID);
	}
    else if (colliderTag == Tag::ROCK || collidedTag == Tag::ROCK)
    {
        auto rockObject = (colliderTag == Tag::ROCK) ? collision.collider : collision.collided;
        auto rock = rockObject->GetComponent<Rock>();

        if (rock->IsFalling()) GetOwner()->SetLocalPosition({ rockObject->GetLocalPosition().x, rockObject->GetLocalPosition().y + 30 });
		else if (rock->IsBreaking())
		{
            m_WasCrushed = true;
            SetState(std::make_unique<PlayerStates::DeathState>());
        }
    }
}

bool Player::CanSwitchMovement(MoveDirection direction)
{
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


bool Player::IsVertical(MoveDirection dir) const
{
    return dir == MoveDirection::Up || dir == MoveDirection::Down;
}

bool Player::IsHorizontal(MoveDirection dir) const
{
    return dir == MoveDirection::Left || dir == MoveDirection::Right;
}


bool Player::CanMoveHorizontal(const SDL_Rect& boundingBox)
{
    glm::vec2 center = {
        boundingBox.x + boundingBox.w / 2.0f,
        boundingBox.y + boundingBox.h / 2.0f
    };

    int index = m_GridPtr->GetCellIndex(center);
    //Don't move when going out of the map
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

bool Player::CanMoveVertical(const SDL_Rect& boundingBox)
{
    glm::vec2 center = {
        boundingBox.x + boundingBox.w / 2.0f,
        boundingBox.y + boundingBox.h / 2.0f
    };

    int index = m_GridPtr->GetCellIndex(center);
    //Don't move when going out of the map
    if (index < 0 || index >= static_cast<int>(m_GridPtr->GetGrid().size())) return false;

    auto cellCenter = m_GridPtr->GetGrid()[index].centerPoint;

    float distanceToCenter = center.y - cellCenter.y;

    if (std::abs(distanceToCenter) <= SNAP_DISTANCE)
    {
        SnapToCellCenter();
        return true;
    }

    float correction = 2 * (distanceToCenter > 0 ? -1.f : 1.f); // move up/down
    auto currentPos = GetOwner()->GetComponent<dae::Transform>()->GetPosition();
    GetOwner()->GetComponent<dae::Transform>()->SetPosition(currentPos.x + correction, currentPos.y);

    return false;
}