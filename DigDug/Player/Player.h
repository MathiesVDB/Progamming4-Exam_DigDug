#pragma once
//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Component.h"
#include "ColliderComponent.h"
#include "RealCollisionSystem.h"
#include "Helpers.h"
#include "PlayerState.h"
class Level;
//-----------------------------------------------------
// Player Class									 
//-----------------------------------------------------
class GridComponent;
class Player final : public dae::Component
{
public:
	enum class MoveAxis
	{
		Horizontal,
		Vertical,
		None
	};

	Player(dae::GameObject* owner, GridComponent* grid);

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	void Update(float deltaTime) override;
	void Render() const override;

	void HandleCollision(const CollisionEvent& collision);
	bool CanSwitchMovement(MoveDirection direction);

	void SnapToCellCenter();

	void SetState(PlayerStates::PlayerState* state);
	void SetLevelPtr(Level* level) { m_LevelPtr = level; }

	GridComponent*	GetGridPtr()  const { return m_GridPtr;  }
	Level*			GetLevelPtr() const { return m_LevelPtr; }


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

	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	MoveAxis					m_CurrentAxis;
	GridComponent*				m_GridPtr;
	Level*						m_LevelPtr; 
	PlayerStates::PlayerState*	m_State;
};
