#include "PookaState.h"
#include <iostream>

#include "Command.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "Pooka.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SpriteComponent.h"

using namespace PookaStates;
//-----------------------------------------------------
// MovingState Class
//-----------------------------------------------------

PookaStates::PookaState* MovingState::Update(Pooka& pooka, float deltaTime)
{
	if (m_HasReachedTarget)
	{
		m_HasReachedTarget = false;
		m_CurrentTarget = FindBestNextTile(pooka);
	}

	SetDirection(pooka);

	MoveTowardsGoal(pooka, deltaTime);

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

	glm::vec2 bestDugTile{};
	float bestDugDist = std::numeric_limits<float>::max();

	glm::vec2 bestUndugTile{};
	float bestUndugDist = std::numeric_limits<float>::max();

	for (const auto& cell : cells)
	{
		float dist = glm::distance(pooka.GetOwner()->GetWorldPosition(), cell.centerPoint);

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

	if (bestUndugDist + 60.f < bestDugDist)
		return bestUndugTile;

	return bestDugTile;
}

std::vector<GridComponent::Cell> MovingState::GetPossibleCells(Pooka& pooka)
{
	auto currentIndex{ pooka.GetGridPtr()->GetCellIndex(pooka.GetOwner()->GetWorldPosition())};
	std::vector<GridComponent::Cell> possibleCells;
	possibleCells.emplace_back(pooka.GetGridPtr()->GetGrid()[currentIndex - pooka.GetGridPtr()->COLUMNS]); // Up
	possibleCells.emplace_back(pooka.GetGridPtr()->GetGrid()[currentIndex + pooka.GetGridPtr()->COLUMNS]); // Down
	possibleCells.emplace_back(pooka.GetGridPtr()->GetGrid()[currentIndex - 1]); // Left
	possibleCells.emplace_back(pooka.GetGridPtr()->GetGrid()[currentIndex + 1]); // Right

	switch (m_Direction)
	{
	case MoveDirection::Left:
		possibleCells.erase(possibleCells.begin() + 3); // erase Right
		break;
	case MoveDirection::Right:
		possibleCells.erase(possibleCells.begin() + 2); // erase Left
		break;
	case MoveDirection::Up:
		possibleCells.erase(possibleCells.begin() + 1); // erase Down
		break;
	case MoveDirection::Down:
		possibleCells.erase(possibleCells.begin() + 0); // erase Up
		break;
	}

	return possibleCells;
}

void MovingState::MoveTowardsGoal(const Pooka& , float )
{
	switch (m_Direction)
	{
	case MoveDirection::Left:
		m_MoveLeftPtr->Execute();
		break;
	case MoveDirection::Right:
		m_MoveRightPtr->Execute();
		break;
	case MoveDirection::Up:
		m_MoveUpPtr->Execute();
		break;
	case MoveDirection::Down:
		m_MoveDownPtr->Execute();
		break;
	}
}

void MovingState::OnEnter(Pooka& pooka)
{
	m_MoveLeftPtr	= new MoveCommand(pooka.GetOwner(), MoveDirection::Left	, true);
	m_MoveRightPtr	= new MoveCommand(pooka.GetOwner(), MoveDirection::Right, true);
	m_MoveUpPtr		= new MoveCommand(pooka.GetOwner(), MoveDirection::Up	, true);
	m_MoveDownPtr	= new MoveCommand(pooka.GetOwner(), MoveDirection::Down	, true);

	m_HasReachedTarget = false;

    m_Sprite = pooka.GetOwner()->GetComponent<SpriteComponent>();

    if (pooka.IsLookingLeft())  m_Sprite->SetNewTexture("Sprites/Pooka/PookaDefaultSprite.png", 2, 5, 5, 6);
    else                        m_Sprite->SetNewTexture("Sprites/Pooka/PookaDefaultSprite.png", 2, 5, 0, 1);
}

void MovingState::OnExit(Pooka&)
{
	// Manual new and delete, because smart pointer would not be deleted on OnExit call
	delete m_MoveLeftPtr;
	delete m_MoveRightPtr;
	delete m_MoveUpPtr;
	delete m_MoveDownPtr;

	m_MoveLeftPtr	= nullptr;
	m_MoveRightPtr	= nullptr;
	m_MoveUpPtr		= nullptr;
	m_MoveDownPtr	= nullptr;
}

//-----------------------------------------------------
// Inflated Class
//-----------------------------------------------------

PookaStates::PookaState* InflatedState::Update(Pooka& pooka, float deltaTime)
{
    if (pooka.GetInflatedState() == m_PreviousState)
    {
        m_ResetTimer += deltaTime;

		if (m_ResetTimer >= RESET_THRESHOLD) return &PookaStates::PookaState::moving;
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
	case Inflated::Exploded:
		m_Sprite->SetSpriteBounds(3, 3, true);
		m_ResetTimer = 0.f;
		pooka.ResetInflation();
    }

    return nullptr;
}

void InflatedState::OnEnter(Pooka& pooka)
{
	m_ResetTimer = 0.f;
	m_Sprite = pooka.GetOwner()->GetComponent<SpriteComponent>();
    if (pooka.IsLookingLeft())  m_Sprite->SetNewTexture("Sprites/Pooka/PookaInflateLeftSprite.png" , 1, 4, 0, 0);
    else                        m_Sprite->SetNewTexture("Sprites/Pooka/PookaInflateRightSprite.png", 1, 4, 0, 0);
}

void InflatedState::OnExit(Pooka& pooka)
{
    pooka.ResetInflation();
}

//-----------------------------------------------------
// DeathState Class
//-----------------------------------------------------

PookaStates::PookaState* DeathState::Update(Pooka& pooka, float deltaTime)
{
	// Give time to show the death animation before removing it from the scene
	m_DeathTimer += deltaTime;

	if (m_DeathTimer >= DEATH_TIME)
    {
        dae::SceneManager::GetInstance().GetActiveScene().MarkForDeletion(pooka.GetOwner());
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

void DeathState::OnExit(Pooka&)
{

}

//-----------------------------------------------------
// GhostState Class
//-----------------------------------------------------

PookaStates::PookaState* GhostState::Update(Pooka& pooka, float deltaTime)
{
	std::cout << "Ghosting" << std::endl;

	glm::vec2 pookaCenter{ m_PookaCollider->GetBoundingBox().x + m_PookaCollider->GetBoundingBox().w / 2.0f,
						   m_PookaCollider->GetBoundingBox().y + m_PookaCollider->GetBoundingBox().h / 2.0f };

	int index{ pooka.GetGridPtr()->GetCellIndex(pookaCenter) };
	if (pooka.GetGridPtr()->GetGrid()[index].hasBeenDug && index != m_StartIndex)
	{
		pooka.GetOwner()->SetLocalPosition(pooka.GetGridPtr()->GetGrid()[index].spawnPosition);
		return &PookaStates::PookaState::moving;
	}

	auto targetPos{ pooka.GetTarget() };
	auto pookaPos { pooka.GetOwner()->GetWorldPosition() };

	glm::vec2 direction = targetPos - pookaPos;

	glm::vec2 normalizedDir = glm::normalize(direction);

	pooka.GetOwner()->SetVelocity(normalizedDir * MOVEMENT_SPEED * deltaTime);

    return nullptr;
}

void GhostState::OnEnter(Pooka& pooka)
{
	m_Sprite = pooka.GetOwner()->GetComponent<SpriteComponent>();
    m_Sprite->SetNewTexture("Sprites/Pooka/PookaDefaultSprite.png", 2, 5, 3, 4);

	m_PookaCollider = pooka.GetOwner()->GetComponent<ColliderComponent>();

	m_StartIndex = pooka.GetGridPtr()->GetCellIndex(pooka.GetOwner()->GetWorldPosition());
}

void GhostState::OnExit(Pooka&)
{
}

//-----------------------------------------------------
// Statics
//-----------------------------------------------------

MovingState     PookaState::moving;
InflatedState   PookaState::inflating;
DeathState      PookaState::dying;
GhostState      PookaState::ghosting;