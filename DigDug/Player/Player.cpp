//---------------------------
// Include Files
//---------------------------
#include "Player.h"
#include "GameObject.h"
#include "GridComponent.h"
//---------------------------
// Constructor & Destructor
//---------------------------
Player::Player(dae::GameObject* owner, GridComponent* grid)
	:	Component(owner),
		m_Direction{MoveDirection::Right},
		m_GridPtr{ grid }
{
    m_State = &PlayerStates::PlayerState::idling;

    SnapToCellCenter();
}

void Player::Update(float deltaTime)
{
    auto newState = m_State->Update(*this, deltaTime);

	if (newState == nullptr) return;
    SetState(newState);
}

void Player::Render() const
{
    m_State->Render(*this);
}

void Player::SetState(PlayerStates::PlayerState* state)
{
    m_State->OnExit(*this);
    m_State = state;
    m_State->OnEnter(*this);
}

void Player::SnapToCellCenter()
{
    if (!m_GridPtr || !GetOwner()->HasComponent<ColliderComponent>())
        return;

    auto boundingBox = GetOwner()->GetComponent<ColliderComponent>()->GetBoundingBox();

    Point2f center = {
        boundingBox.x + boundingBox.w / 2.0f,
        boundingBox.y + boundingBox.h / 2.0f
    };

    int index = m_GridPtr->GetCellIndex(center);
    auto cellCenter = m_GridPtr->GetGrid()[index].centerPoint;

    float newX = cellCenter.x - boundingBox.w / 2.0f;
    float newY = cellCenter.y - boundingBox.h / 2.0f;

    GetOwner()->GetComponent<dae::Transform>()->SetPosition(newX, newY, 0.0f);
}

void Player::HandleCollision(const CollisionEvent& )
{
	
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
    Point2f center = {
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
    GetOwner()->GetComponent<dae::Transform>()->SetPosition(currentPos.x, currentPos.y + correction, currentPos.z);

    return false;
}

bool Player::CanMoveVertical(const SDL_Rect& boundingBox)
{
    Point2f center = {
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
    GetOwner()->GetComponent<dae::Transform>()->SetPosition(currentPos.x + correction, currentPos.y, currentPos.z);

    return false;
}