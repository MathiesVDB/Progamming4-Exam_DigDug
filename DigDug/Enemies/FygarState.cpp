#include "FygarState.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "Fygar.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SpriteComponent.h"
#include "Command.h"

using namespace FygarStates;
//-----------------------------------------------------
// MovingState Class
//-----------------------------------------------------

std::unique_ptr<FygarState> MovingState::Update(Fygar& fygar, float deltaTime)
{
	if		( fygar.IsLookingLeft() && !m_Sprite->IsAlreadyWithinBounds(8, 9)) m_Sprite->SetSpriteBounds(8, 9, true);
	else if (!fygar.IsLookingLeft() && !m_Sprite->IsAlreadyWithinBounds(0, 1)) m_Sprite->SetSpriteBounds(0, 1, true);

	if (!fygar.IsControlled()) return AILogic(fygar, deltaTime);

	m_AccumulatedGhostTimer += deltaTime;

	if (m_AccumulatedGhostTimer <= GHOST_DELAY) return nullptr;

	auto boundingBox = m_Collider->GetBoundingBox();

	glm::vec2 center{
			boundingBox.x + boundingBox.w / 2.0f,
			boundingBox.y + boundingBox.h / 2.0f
	};

	int index{ fygar.GetGridPtr()->GetCellIndex(glm::vec2(center.x - boundingBox.w / 4.f, center.y - boundingBox.h / 4.0f)) };
	if (!fygar.GetGridPtr()->GetGrid()[index].hasBeenDug) return std::make_unique<GhostState>();

	index = fygar.GetGridPtr()->GetCellIndex(glm::vec2(center.x + boundingBox.w / 4.f, center.y + boundingBox.h / 4.0f));
	if (!fygar.GetGridPtr()->GetGrid()[index].hasBeenDug) return std::make_unique<GhostState>();

	return nullptr;
}

std::unique_ptr<FygarState> MovingState::AILogic(Fygar& fygar, float deltaTime)
{
	if (fygar.GetOwner()->HasComponent<Fygar>() && !m_AttackCommand) m_AttackCommand = std::make_unique<AttackCommand>(fygar.GetOwner());

	auto fygarPos{ fygar.GetOwner()->GetWorldPosition() };

	if (m_AccumulatedTime >= GHOST_TIMER && std::abs(fygar.GetTarget().y - fygarPos.y) <= 10.f)
	{
		m_AttackCommand->Execute();
		return nullptr;
	}

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

		if (fygar.IsFleeing())
		{
			glm::vec2 tempTarget{ fygar.GetTarget() };

			tempTarget.x -= m_Collider->GetBoundingBox().w / 2.f;
			tempTarget.y -= m_Collider->GetBoundingBox().h / 2.f;

			if (glm::distance(tempTarget, fygar.GetOwner()->GetWorldPosition()) <= SNAP_DISTANCE)
			{
				dae::SceneManager::GetInstance().GetActiveScene().MarkForDeletion(fygar.GetOwner());
				GameDirector::GetInstance().SwitchToNextScene(); //Do this directly to prevent points being rewarded
				return nullptr;
			}
		}

		m_HasReachedTarget = false;
		m_CurrentTarget = FindBestNextTile(fygar);

		int index{ fygar.GetGridPtr()->GetCellIndex(m_CurrentTarget) };
		if (!fygar.GetGridPtr()->GetGrid()[index].hasBeenDug) return std::make_unique<GhostState>();

		// Adjust for fygar size
		m_CurrentTarget.x -= m_Collider->GetBoundingBox().w / 2.f;
		m_CurrentTarget.y -= m_Collider->GetBoundingBox().h / 2.f;
	}

	SetDirection(fygar);

	MoveTowardsGoal(fygar, deltaTime);

	return nullptr;
}

void MovingState::SetDirection(Fygar& fygar)
{
	glm::vec2 pookaPos{ fygar.GetOwner()->GetWorldPosition() };
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

glm::vec2 MovingState::FindBestNextTile(Fygar& fygar)
{
	auto cells = GetPossibleCells(fygar);

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

		cells = GetPossibleCells(fygar);
	}

	glm::vec2 bestDugTile{};
	float bestDugDist = std::numeric_limits<float>::max();

	glm::vec2 bestUndugTile{};
	float bestUndugDist = std::numeric_limits<float>::max();

	for (const auto& cell : cells)
	{
		float dist = glm::distance(fygar.GetTarget(), cell.centerPoint);

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

std::vector<GridComponent::Cell> MovingState::GetPossibleCells(Fygar& fygar)
{
	auto currentIndex{ fygar.GetGridPtr()->GetCellIndex(fygar.GetOwner()->GetWorldPosition()) };
	int possibleIndex{};
	std::vector<GridComponent::Cell> possibleCells;

	auto tryAddCell = [&](int possibleIndex, MoveDirection forbiddenDir)
		{
			if (possibleIndex > -1 && m_Direction != forbiddenDir && fygar.GetGridPtr()->GetGrid()[possibleIndex].centerPoint != m_BlockedTarget)
				possibleCells.emplace_back(fygar.GetGridPtr()->GetGrid()[possibleIndex]);
		};

	possibleIndex = currentIndex - fygar.GetGridPtr()->GetColumns(); // Up
	tryAddCell(possibleIndex, MoveDirection::Down);

	possibleIndex = currentIndex + fygar.GetGridPtr()->GetColumns(); // Down
	tryAddCell(possibleIndex, MoveDirection::Up);

	possibleIndex = currentIndex - 1; // Left
	tryAddCell(possibleIndex, MoveDirection::Right);

	possibleIndex = currentIndex + 1; // Right
	tryAddCell(possibleIndex, MoveDirection::Left);

	return possibleCells;
}

void MovingState::MoveTowardsGoal(const Fygar& fygar, float)
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

	if (glm::distance(fygar.GetOwner()->GetWorldPosition(), m_CurrentTarget) <= SNAP_DISTANCE)
	{
		fygar.GetOwner()->SetLocalPosition(m_CurrentTarget);
		m_HasReachedTarget = true;
	}
}

void MovingState::OnEnter(Fygar& fygar)
{
	// Get components here so it doesn't happen every frame
	m_Sprite	= fygar.GetOwner()->GetComponent<SpriteComponent>();
	m_Collider	= fygar.GetOwner()->GetComponent<ColliderComponent>();

	//Setup animation
	if (fygar.IsLookingLeft())  m_Sprite->SetNewTexture("Sprites/Fygar/FygarDefaultSprite.png", 2, 8, 8, 9);
	else                        m_Sprite->SetNewTexture("Sprites/Fygar/FygarDefaultSprite.png", 2, 8, 0, 1);

	if (fygar.IsControlled()) return;

	//Setup commands
	m_MoveLeftUPtr	= std::make_unique<MoveCommand>(fygar.GetOwner(), MoveDirection::Left	, MOVEMENT_SPEED, true);
	m_MoveRightUPtr = std::make_unique<MoveCommand>(fygar.GetOwner(), MoveDirection::Right	, MOVEMENT_SPEED, true);
	m_MoveUpUPtr	= std::make_unique<MoveCommand>(fygar.GetOwner(), MoveDirection::Up		, MOVEMENT_SPEED, true);
	m_MoveDownUPtr	= std::make_unique<MoveCommand>(fygar.GetOwner(), MoveDirection::Down	, MOVEMENT_SPEED, true);

	if (fygar.GetOwner()->HasComponent<Fygar>()) m_AttackCommand = std::make_unique<AttackCommand>(fygar.GetOwner());

	// Reset private member variables
	m_HasReachedTarget	= true;
	m_AccumulatedTime	= 0;
}

void MovingState::OnExit(Fygar&)
{
	m_AccumulatedTime = 0;
}

//-----------------------------------------------------
// Inflated Class
//-----------------------------------------------------

std::unique_ptr<FygarState> InflatedState::Update(Fygar& fygar, float deltaTime)
{
	if (fygar.GetInflatedState() == Inflated::Exploded)
	{
		m_Sprite->SetSpriteBounds(3, 3, true);
		m_ResetTimer = 0.f;
		return std::make_unique<DeathState>();
	}

	if (fygar.GetInflatedState() == m_PreviousState)
	{
		m_ResetTimer += deltaTime;

		if (m_ResetTimer >= RESET_THRESHOLD) fygar.DecreaseInflation();
		return nullptr;
	}

	m_PreviousState = fygar.GetInflatedState();

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

void InflatedState::OnEnter(Fygar& fygar)
{
	m_ResetTimer = 0.f;
	m_Sprite = fygar.GetOwner()->GetComponent<SpriteComponent>();

	m_StartPos = fygar.GetOwner()->GetWorldPosition();

	m_Sprite->SetNewTexture("Sprites/Fygar/FygarInflateRightSprite.png", 1, 4, 0, 0);
}

void InflatedState::OnExit(Fygar& fygar)
{
	if (m_PreviousState != Inflated::Exploded)
	{
		fygar.GetOwner()->SetLocalPosition(m_StartPos);
	}
}

//-----------------------------------------------------
// DeathState Class
//-----------------------------------------------------

std::unique_ptr<FygarState> DeathState::Update(Fygar& fygar, float deltaTime)
{
    // Give time to show the death animation before removing it from the scene
    m_DeathTimer += deltaTime;

    if (m_DeathTimer >= DEATH_TIME)
    {
        dae::SceneManager::GetInstance().GetActiveScene().MarkForDeletion(fygar.GetOwner());
		fygar.NotifyDeath();
        return nullptr;
    }

    return nullptr;
}

void DeathState::OnEnter(Fygar& fygar)
{
    m_Sprite = fygar.GetOwner()->GetComponent<SpriteComponent>();

    if (!fygar.WasCrushed()) return;

    if (fygar.IsLookingLeft()) m_Sprite->SetNewTexture("Sprites/Fygar/FygarDefaultSprite.png", 2, 8,  2,  2);
    else                       m_Sprite->SetNewTexture("Sprites/Fygar/FygarDefaultSprite.png", 2, 8, 10, 10);
}

void DeathState::OnExit(Fygar& )
{
}

//-----------------------------------------------------
// GhostState Class
//-----------------------------------------------------

std::unique_ptr<FygarState> GhostState::Update(Fygar& fygar, float deltaTime)
{
	if (!fygar.IsControlled()) return AILogic(fygar, deltaTime);

	m_AccumulatedGhostTimer += deltaTime;

	if (m_AccumulatedGhostTimer <= GHOST_DELAY) return nullptr;

	auto boundingBox = m_FygarCollider->GetBoundingBox();

	glm::vec2 center{
			boundingBox.x + boundingBox.w / 2.0f,
			boundingBox.y + boundingBox.h / 2.0f
	};

	int indexA = fygar.GetGridPtr()->GetCellIndex(glm::vec2(center.x - boundingBox.w / 4.f, center.y - boundingBox.h / 4.0f));
	int indexB = fygar.GetGridPtr()->GetCellIndex(glm::vec2(center.x + boundingBox.w / 4.f, center.y + boundingBox.h / 4.0f));

	// If either cell is dug and not the start index, transition to MovingState
	if ((fygar.GetGridPtr()->GetGrid()[indexA].hasBeenDug && indexA != m_StartIndex) ||
		(fygar.GetGridPtr()->GetGrid()[indexB].hasBeenDug && indexB != m_StartIndex))
	{
		return std::make_unique<MovingState>();
	}

	//Allow rematerialising on startindex after fygar has left it
	if (indexA != m_StartIndex && indexB != m_StartIndex)
	{
		m_StartIndex = -1;
	}

    return nullptr;
}

std::unique_ptr<FygarState> GhostState::AILogic(Fygar& fygar, float)
{
	glm::vec2 pookaCenter{ m_FygarCollider->GetBoundingBox().x + m_FygarCollider->GetBoundingBox().w / 2.0f,
						   m_FygarCollider->GetBoundingBox().y + m_FygarCollider->GetBoundingBox().h / 2.0f };

	int index{ fygar.GetGridPtr()->GetCellIndex(pookaCenter) };
	index = std::max(index, 0);
	if (fygar.GetGridPtr()->GetGrid()[index].hasBeenDug && index != m_StartIndex)
	{
		m_IsRematerialising = true;
	}

	if (glm::distance(fygar.GetGridPtr()->GetGrid()[index].centerPoint, pookaCenter) <= SNAP_DISTANCE && m_IsRematerialising)
	{
		return std::make_unique<MovingState>();
	}

	auto targetPos{ fygar.GetTarget() };
	auto pookaPos{ fygar.GetOwner()->GetWorldPosition() };

	if (m_IsRematerialising)
	{
		glm::vec2 newTarget{ fygar.GetGridPtr()->GetGrid()[index].centerPoint };

		targetPos = {
			newTarget.x - m_FygarCollider->GetBoundingBox().w / 2.f,
			newTarget.y - m_FygarCollider->GetBoundingBox().h / 2.f };
	}

	glm::vec2 direction = targetPos - pookaPos;

	glm::vec2 normalizedDir = glm::normalize(direction);

	fygar.GetOwner()->SetVelocity(normalizedDir * MOVEMENT_SPEED);

	return nullptr;
}

void GhostState::OnEnter(Fygar& fygar)
{
    m_IsRematerialising = false;

    m_Sprite = fygar.GetOwner()->GetComponent<SpriteComponent>();
    m_Sprite->SetNewTexture("Sprites/Fygar/FygarDefaultSprite.png", 2, 8, 6, 7);

    m_FygarCollider = fygar.GetOwner()->GetComponent<ColliderComponent>();

    m_StartIndex = fygar.GetGridPtr()->GetCellIndex(fygar.GetOwner()->GetWorldPosition());
}

void GhostState::OnExit(Fygar& fygar)
{
	m_AccumulatedGhostTimer = 0;

	if (fygar.IsControlled()) return;

	//Set position to center
	int index{ fygar.GetGridPtr()->GetCellIndex(fygar.GetOwner()->GetLocalPosition()) };
	auto cellCenter{ fygar.GetGridPtr()->GetGrid()[index].centerPoint };

	glm::vec2 snapPosition{
		cellCenter.x - m_FygarCollider->GetBoundingBox().w / 2.f,
		cellCenter.y - m_FygarCollider->GetBoundingBox().h / 2.f
	};

	fygar.GetOwner()->SetLocalPosition(snapPosition);
}

//-----------------------------------------------------
// AttackState Class
//-----------------------------------------------------

std::unique_ptr<FygarState> AttackState::Update(Fygar& fygar, float deltaTime)
{
	if (fygar.IsLookingLeft())
	{
		auto fygarPos{ fygar.GetOwner()->GetWorldPosition() };
		m_AttackObject->SetLocalPosition({ fygarPos.x - m_AttackSprite->GetSpriteSize().x, fygarPos.y });
	}

	m_AccumulatedTime += deltaTime;

	if (m_AccumulatedTime >= ATTACK_TIME) fygar.SetState(std::make_unique<MovingState>());

	if (fygar.IsLookingLeft())	fygar.GetOwner()->GetComponent<SpriteComponent>()->SetSpriteBounds(8, 8, true);
	else						fygar.GetOwner()->GetComponent<SpriteComponent>()->SetSpriteBounds(0, 0, true);

    return nullptr;
}

void AttackState::OnEnter(Fygar& fygar)
{
	m_Sprite	= fygar.GetOwner()->GetComponent<SpriteComponent>();
	m_Collider	= fygar.GetOwner()->GetComponent<ColliderComponent>();

	CreateFireGameObject(fygar);

	if (fygar.IsLookingLeft()) m_Sprite->SetNewTexture("Sprites/Fygar/FygarDefaultSprite.png", 2, 8, 8, 8);
	else					   m_Sprite->SetNewTexture("Sprites/Fygar/FygarDefaultSprite.png", 2, 8, 0, 0);

	fygar.NotifyAttack();
}

void AttackState::CreateFireGameObject(Fygar& fygar)
{
	auto fygarPos{ fygar.GetOwner()->GetWorldPosition() };

	auto FygarAttackGameObject = std::make_unique<dae::GameObject>();
	m_AttackObject = FygarAttackGameObject.get();

	if (fygar.IsLookingLeft())	FygarAttackGameObject->AddComponent<SpriteComponent>("Sprites/Fygar/FireSprite.png", 2, 6, ATTACK_TIME / 3, 6, 11, true);
	else						FygarAttackGameObject->AddComponent<SpriteComponent>("Sprites/Fygar/FireSprite.png", 2, 6, ATTACK_TIME / 3, 0, 2, true);

	FygarAttackGameObject->AddComponent<ColliderComponent>(ENEMY_ENTITY);

	if (fygar.IsLookingLeft())	FygarAttackGameObject->SetLocalPosition({fygarPos.x - m_Collider->GetBoundingBox().w, fygarPos.y});
	else						FygarAttackGameObject->SetLocalPosition({fygarPos.x + m_Collider->GetBoundingBox().w, fygarPos.y});

	FygarAttackGameObject->SetRenderLayer(2);
	FygarAttackGameObject->SetObjectTag("Entity");

	m_AttackSprite = m_AttackObject->GetComponent<SpriteComponent>();

	dae::SceneManager::GetInstance().GetActiveScene().MarkForAdd(std::move(FygarAttackGameObject));
}

void AttackState::OnExit(Fygar&)
{
	if (m_AttackObject)
	{
		dae::SceneManager::GetInstance().GetActiveScene().MarkForDeletion(m_AttackObject);
		m_AttackObject = nullptr;
		m_AttackSprite = nullptr;
	}
}