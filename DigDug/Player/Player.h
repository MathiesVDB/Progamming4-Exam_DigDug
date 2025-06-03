#pragma once
//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Component.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "RealCollisionSystem.h"
#include "Helpers.h"
#include "PlayerState.h"
#include "HealthComponent.h"

class RopeComponent;
class Level;
class GridComponent;
class Player final : public dae::Component
{
public:
	Player(dae::GameObject* owner, GridComponent* grid);

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	void Update(float deltaTime) override;
	void Render() const override;

	void Attack();

	void HandleCollision(const CollisionEvent& collision);
	bool CanSwitchMovement(MoveDirection direction);

	void SnapToCellCenter();

	void SetState(PlayerStates::PlayerState* state);
	void SetLevelPtr(Level* level) { m_LevelPtr = level; }
	void ResetPlayer() const { GetOwner()->SetLocalPosition(m_SpawnPosition); }

	GridComponent*			GetGridPtr()	const { return m_GridPtr;	}
	RopeComponent*			GetRopePtr()	const { return m_Rope;		}
	Level*					GetLevelPtr()	const { return m_LevelPtr;	}
	MoveDirection			GetDirection()	const { return m_Direction;	}
	ColliderComponent*		GetCollider()	const { return m_Collider;	}
	HealthComponent*		GetHealth()		const { return m_Health;	}

	bool					WasCrushed()	const { return m_WasCrushed; }
	bool					IsDead()		const { return m_State == &PlayerStates::PlayerState::dying; }

	//-------------------------------------------------
	// Constants					
	//-------------------------------------------------
	static constexpr int SNAP_DISTANCE = 1;
private:
	//-------------------------------------------------
	// Private member functions								
	//-------------------------------------------------
	bool CanMoveHorizontal(const SDL_Rect& boundingBox);
	bool CanMoveVertical(const SDL_Rect& boundingBox);

	bool IsHorizontal(MoveDirection dir) const;
	bool IsVertical(MoveDirection dir) const;

	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	bool m_WasCrushed{ false };

	glm::vec2 m_SpawnPosition{};

	GridComponent*				m_GridPtr;
	RopeComponent*				m_Rope;
	Level*						m_LevelPtr; 
	PlayerStates::PlayerState*	m_State;
	ColliderComponent*			m_Collider;
	MoveDirection				m_Direction;
	HealthComponent*			m_Health;
};
