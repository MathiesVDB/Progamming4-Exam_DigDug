#include "PookaState.h"
#include <algorithm>
#include <iostream>
#include "GameObject.h"
#include "GridComponent.h"
#include "Pooka.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SpriteComponent.h"

using namespace PookaStates;
//-----------------------------------------------------
// MovingState Class
//-----------------------------------------------------

void MovingState::Render(const Pooka& ) const
{
}

std::unique_ptr<PookaState> MovingState::Update(Pooka& pooka, float deltaTime)
{
	m_AccumulatedTime += deltaTime;
	m_StuckTimer += deltaTime;

	if (m_StuckTimer >= PREVENT_STUCK_TIMER)
	{
		m_CurrentTarget.x += m_Collider->GetBoundingBox().w / 2.f;
		m_CurrentTarget.y += m_Collider->GetBoundingBox().h / 2.f;

		m_BlockedTarget = m_CurrentTarget;
		m_HasReachedTarget = true;
	}

	if (m_HasReachedTarget)
	{
		m_StuckTimer = 0;

		if (pooka.IsFleeing())
		{
			glm::vec2 tempTarget{ pooka.GetTarget() };

			tempTarget.x -= m_Collider->GetBoundingBox().w / 2.f;
			tempTarget.y -= m_Collider->GetBoundingBox().h / 2.f;

			if (glm::distance(tempTarget, pooka.GetOwner()->GetWorldPosition()) <= SNAP_DISTANCE)
			{
				dae::SceneManager::GetInstance().GetActiveScene().MarkForDeletion(pooka.GetOwner());
				GameDirector::GetInstance().SwitchToNextScene(); //Do this directly to prevent points being rewarded
				return nullptr;
			}
		}

		m_HasReachedTarget = false;
		m_CurrentTarget = FindBestNextTile(pooka);

		int index{ pooka.GetGridPtr()->GetCellIndex(m_CurrentTarget) };
		if (!pooka.GetGridPtr()->GetGrid()[index].hasBeenDug) return std::make_unique<GhostState>();

		// Adjust for pooka size
		m_CurrentTarget.x -= m_Collider->GetBoundingBox().w / 2.f;
		m_CurrentTarget.y -= m_Collider->GetBoundingBox().h / 2.f;
	}

	SetDirection(pooka);

	MoveTowardsGoal(pooka, deltaTime);

	if (pooka.IsLookingLeft() && !m_Sprite->IsAlreadyWithinBounds(5, 6))		m_Sprite->SetSpriteBounds(5, 6, true);
	else if (!pooka.IsLookingLeft() && !m_Sprite->IsAlreadyWithinBounds(0, 1))m_Sprite->SetSpriteBounds(0, 1, true);

    return nullptr;
}

void MovingState::SetDirection(Pooka& pooka)
{
	glm::vec2 pookaPos{ pooka.GetOwner()->GetWorldPosition() };
	glm::vec2 targetPos{ m_CurrentTarget };

	glm::vec2 direction = targetPos - pookaPos;

	if (glm::abs(direction.x) > glm::abs(direction.y))
	{
		if (direction.x < 0) m_Direction = MoveDirection::Left; 
		else m_Direction = MoveDirection::Right;

		return;
	}

	if (direction.y < 0) m_Direction = MoveDirection::Up; 
	else m_Direction = MoveDirection::Down; 
}

glm::vec2 MovingState::FindBestNextTile(Pooka& pooka)
{
	auto cells = GetPossibleCells(pooka);

	// Check if all available cells are unDug
	bool allUndug = std::all_of(cells.begin(), cells.end(), [](const GridComponent::Cell& cell) {
		return !cell.hasBeenDug;
		});

	// Reverse direction if no dug cells exist
	if (allUndug)
	{
		switch (m_Direction)
		{
		case MoveDirection::Left:  m_Direction = MoveDirection::Right; break;
		case MoveDirection::Right: m_Direction = MoveDirection::Left;  break;
		case MoveDirection::Up:    m_Direction = MoveDirection::Down;  break;
		case MoveDirection::Down:  m_Direction = MoveDirection::Up;    break;
		}

		cells = GetPossibleCells(pooka);
	}

	glm::vec2 bestDugTile{};
	float bestDugDist = std::numeric_limits<float>::max();

	glm::vec2 bestUndugTile{};
	float bestUndugDist = std::numeric_limits<float>::max();

	for (const auto& cell : cells)
	{
		float dist = glm::distance(pooka.GetTarget(), cell.centerPoint);
		
		if (cell.hasBeenDug)
		{
			if (dist < SNAP_DISTANCE)
				return cell.centerPoint;

			if (dist < bestDugDist)
			{
				bestDugDist = dist;
				bestDugTile = cell.centerPoint;
			}
		}
		else
		{
			if (dist < bestUndugDist)
			{
				bestUndugDist = dist;
				bestUndugTile = cell.centerPoint;
			}
		}
	}

	if (bestUndugDist + 40.f < bestDugDist && m_AccumulatedTime >= GHOST_TIMER)
	{
		return bestUndugTile;
	}
	return bestDugTile;
}

std::vector<GridComponent::Cell> MovingState::GetPossibleCells(Pooka& pooka)
{
	auto currentIndex{ pooka.GetGridPtr()->GetCellIndex(pooka.GetOwner()->GetWorldPosition())};
	int possibleIndex{};
	std::vector<GridComponent::Cell> possibleCells;

	auto tryAddCell = [&](int possibleIndex, MoveDirection forbiddenDir)
		{
			if (possibleIndex > -1 && m_Direction != forbiddenDir && pooka.GetGridPtr()->GetGrid()[possibleIndex].centerPoint != m_BlockedTarget)
				possibleCells.emplace_back(pooka.GetGridPtr()->GetGrid()[possibleIndex]);
		};

	possibleIndex = currentIndex - pooka.GetGridPtr()->GetColumns(); // Up
	tryAddCell(possibleIndex, MoveDirection::Down);

	possibleIndex = currentIndex + pooka.GetGridPtr()->GetColumns(); // Down
	tryAddCell(possibleIndex, MoveDirection::Up);

	possibleIndex = currentIndex - 1; // Left
	tryAddCell(possibleIndex, MoveDirection::Right);

	possibleIndex = currentIndex + 1; // Right
	tryAddCell(possibleIndex, MoveDirection::Left);

	return possibleCells;
}

void MovingState::MoveTowardsGoal(const Pooka& pooka, float )
{
	switch (m_Direction)
	{
	case MoveDirection::Left:
		m_MoveLeftUPtr->Execute();
		break;
	case MoveDirection::Right:
		m_MoveRightUPtr->Execute();
		break;
	case MoveDirection::Up:
		m_MoveUpUPtr->Execute();
		break;
	case MoveDirection::Down:
		m_MoveDownUPtr->Execute();
		break;
	}

	if (glm::distance(pooka.GetOwner()->GetWorldPosition(), m_CurrentTarget) <= SNAP_DISTANCE)
	{
		pooka.GetOwner()->SetLocalPosition(m_CurrentTarget);
		m_HasReachedTarget = true;
	}
}

void MovingState::OnEnter(Pooka& pooka)
{
	//Setup movement commands
	m_MoveLeftUPtr	= std::make_unique<MoveCommand>(pooka.GetOwner(), MoveDirection::Left , MOVEMENT_SPEED, true);
	m_MoveRightUPtr	= std::make_unique<MoveCommand>(pooka.GetOwner(), MoveDirection::Right, MOVEMENT_SPEED, true);
	m_MoveUpUPtr	= std::make_unique<MoveCommand>(pooka.GetOwner(), MoveDirection::Up	  , MOVEMENT_SPEED, true);
	m_MoveDownUPtr	= std::make_unique<MoveCommand>(pooka.GetOwner(), MoveDirection::Down , MOVEMENT_SPEED, true);

	// Reset private member variables
	m_HasReachedTarget = true;
	m_AccumulatedTime = 0;

	// Get components here so it doesn't happen every frame
    m_Sprite = pooka.GetOwner()->GetComponent<SpriteComponent>();
	m_Collider = pooka.GetOwner()->GetComponent<ColliderComponent>();

	//Setup animation
    if (pooka.IsLookingLeft())  m_Sprite->SetNewTexture("Sprites/Pooka/PookaDefaultSprite.png", 2, 5, 5, 6);
    else                        m_Sprite->SetNewTexture("Sprites/Pooka/PookaDefaultSprite.png", 2, 5, 0, 1);
}

void MovingState::OnExit(Pooka&)
{
}

//-----------------------------------------------------
// Inflated Class
//-----------------------------------------------------

void InflatedState::Render(const Pooka& ) const
{
	SDL_Renderer* sdlRenderer = dae::Renderer::GetInstance().GetSDLRenderer();
	SDL_SetRenderDrawColor(sdlRenderer, 255, 0, 0, 255);

	SDL_Rect cellRect{
		static_cast<int>(m_StartPos.x),
		static_cast<int>(m_StartPos.y),
		10,
		10 };

	SDL_RenderFillRect(sdlRenderer, &cellRect);
}

std::unique_ptr<PookaState> InflatedState::Update(Pooka& pooka, float deltaTime)
{
	if (pooka.GetInflatedState() == Inflated::Exploded)
	{
		m_Sprite->SetSpriteBounds(3, 3, true);
		m_ResetTimer = 0.f;
		return std::make_unique<DeathState>();
	}

    if (pooka.GetInflatedState() == m_PreviousState)
    {
        m_ResetTimer += deltaTime;

		if (m_ResetTimer >= RESET_THRESHOLD) pooka.DecreaseInflation();
		return nullptr;
    }

	m_PreviousState = pooka.GetInflatedState();

    switch (m_PreviousState)
    {
	case Inflated::Stage1:
		m_Sprite->SetSpriteBounds(0, 0, true);
		m_ResetTimer = 0.f;
		break;
	case Inflated::Stage2:
		m_Sprite->SetSpriteBounds(1, 1, true);
		m_ResetTimer = 0.f;
		break;
	case Inflated::Stage3:
		m_Sprite->SetSpriteBounds(2, 2, true);
		m_ResetTimer = 0.f;
		break;
    }

    return nullptr;
}

void InflatedState::OnEnter(Pooka& pooka)
{
	m_ResetTimer = 0.f;
	m_Sprite = pooka.GetOwner()->GetComponent<SpriteComponent>();

	m_StartPos = pooka.GetOwner()->GetWorldPosition();

    m_Sprite->SetNewTexture("Sprites/Pooka/PookaInflateRightSprite.png", 1, 4, 0, 0);
}

void InflatedState::OnExit(Pooka& pooka)
{
	if (m_PreviousState != Inflated::Exploded)
	{
		pooka.GetOwner()->SetLocalPosition(m_StartPos);
	}
}

//-----------------------------------------------------
// DeathState Class
//-----------------------------------------------------

void DeathState::Render(const Pooka& ) const
{
}

std::unique_ptr<PookaState> DeathState::Update(Pooka& pooka, float deltaTime)
{
	// Give time to show the death animation before removing it from the scene
	m_DeathTimer += deltaTime;

	if (m_DeathTimer >= DEATH_TIME)
    {
        dae::SceneManager::GetInstance().GetActiveScene().MarkForDeletion(pooka.GetOwner());
		pooka.NotifyDeath();
        return nullptr;
    }

    return nullptr;
}

void DeathState::OnEnter(Pooka& pooka)
{
	m_Sprite = pooka.GetOwner()->GetComponent<SpriteComponent>();

    if (!pooka.WasCrushed()) return;

    if  (pooka.IsLookingLeft()) m_Sprite->SetNewTexture("Sprites/Pooka/PookaDefaultSprite.png", 2, 5, 2, 2);
    else                        m_Sprite->SetNewTexture("Sprites/Pooka/PookaDefaultSprite.png", 2, 5, 7, 7);
}

void DeathState::OnExit(Pooka& )
{
}

//-----------------------------------------------------
// GhostState Class
//-----------------------------------------------------

void GhostState::Render(const Pooka& ) const
{
	
}

std::unique_ptr<PookaState> GhostState::Update(Pooka& pooka, float )
{
	glm::vec2 pookaCenter{ m_PookaCollider->GetBoundingBox().x + m_PookaCollider->GetBoundingBox().w / 2.0f,
						   m_PookaCollider->GetBoundingBox().y + m_PookaCollider->GetBoundingBox().h / 2.0f };

	int index{ pooka.GetGridPtr()->GetCellIndex(pookaCenter) };
	index = std::max(index, 0);
	if (pooka.GetGridPtr()->GetGrid()[index].hasBeenDug && index != m_StartIndex)
	{
		m_IsRematerialising = true;
	}

	if (glm::distance(pooka.GetGridPtr()->GetGrid()[index].centerPoint, pookaCenter) <= SNAP_DISTANCE && m_IsRematerialising)
	{
		return std::make_unique<MovingState>();
	}

	auto targetPos{ pooka.GetTarget() };
	auto pookaPos { pooka.GetOwner()->GetWorldPosition() };

	if (m_IsRematerialising)
	{
		glm::vec2 newTarget{ pooka.GetGridPtr()->GetGrid()[index].centerPoint };

		targetPos = {
			newTarget.x - m_PookaCollider->GetBoundingBox().w / 2.f,
			newTarget.y - m_PookaCollider->GetBoundingBox().h / 2.f };
	}

	glm::vec2 direction = targetPos - pookaPos;

	glm::vec2 normalizedDir = glm::normalize(direction);

	pooka.GetOwner()->SetVelocity(normalizedDir * MOVEMENT_SPEED);

    return nullptr;
}

void GhostState::OnEnter(Pooka& pooka)
{
	m_IsRematerialising = false;

	m_Sprite = pooka.GetOwner()->GetComponent<SpriteComponent>();
    m_Sprite->SetNewTexture("Sprites/Pooka/PookaDefaultSprite.png", 2, 5, 3, 4);

	m_PookaCollider = pooka.GetOwner()->GetComponent<ColliderComponent>();

	m_StartIndex = pooka.GetGridPtr()->GetCellIndex(pooka.GetOwner()->GetWorldPosition());
}

void GhostState::OnExit(Pooka& pooka)
{
	//Set position to center
	int index{ pooka.GetGridPtr()->GetCellIndex(pooka.GetOwner()->GetLocalPosition()) };
	auto cellCenter{ pooka.GetGridPtr()->GetGrid()[index].centerPoint };

	glm::vec2 snapPosition{
		cellCenter.x - m_PookaCollider->GetBoundingBox().w / 2.f,
		cellCenter.y - m_PookaCollider->GetBoundingBox().h / 2.f,
	};

	pooka.GetOwner()->SetLocalPosition(snapPosition);
}