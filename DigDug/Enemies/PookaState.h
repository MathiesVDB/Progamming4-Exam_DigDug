#pragma once
#include <vec2.hpp>
#include <vector>

#include "GridComponent.h"
#include "Helpers.h"

class MoveCommand;
class GridComponent;
struct Cell;
class ColliderComponent;
class SpriteComponent;
class Pooka;

namespace PookaStates
{
    class MovingState;
    class InflatedState;
    class DeathState;
    class GhostState;
    
    class PookaState 
    {
    public:
        static MovingState      moving;
        static InflatedState    inflating;
        static DeathState       dying;
        static GhostState       ghosting;
    
        virtual ~PookaState() {}
        virtual PookaStates::PookaState* Update(Pooka&, float) { return nullptr; }
    
        virtual void OnEnter(Pooka& ) {}
    	virtual void OnExit(Pooka& ) {}

        const float MOVEMENT_SPEED{ 1000.f };
        const int   SNAP_DISTANCE {    1   };

    protected:
        SpriteComponent* m_Sprite{ nullptr };
		glm::vec2 m_Target{};
    };
    
    class MovingState : public PookaState
    {
    public:
        PookaStates::PookaState* Update(Pooka& pooka, float deltaTime) override;
    
    	void OnEnter(Pooka& pooka) override;
    	void OnExit(Pooka& pooka) override;

    private:
        // Private member functions
        glm::vec2 FindBestNextTile(Pooka& pooka);
        void SetDirection(Pooka& pooka);
        std::vector<GridComponent::Cell> GetPossibleCells(Pooka& pooka);
        void MoveTowardsGoal(const Pooka& pooka, float deltaTime);
        // Private member variables
		MoveDirection m_Direction{ MoveDirection::Right };
		glm::vec2 m_CurrentTarget{};
        bool m_HasReachedTarget{};

        //Commands
        MoveCommand* m_MoveLeftPtr  { nullptr };
        MoveCommand* m_MoveRightPtr { nullptr };
        MoveCommand* m_MoveUpPtr    { nullptr };
        MoveCommand* m_MoveDownPtr  { nullptr };
    };

    class InflatedState : public PookaState
    {
    public:
        PookaStates::PookaState* Update(Pooka& pooka, float deltaTime) override;
    
        void OnEnter(Pooka& pooka) override;
        void OnExit(Pooka& pooka) override;

        const float RESET_THRESHOLD{ 1.f }; // Time player has to inflate again before inflation is reset

    private:
        float m_ResetTimer{};

		Inflated m_PreviousState{ Inflated::None }; 
    };
    
    class DeathState : public PookaState
    {
    public:
        PookaStates::PookaState* Update(Pooka& pooka, float deltaTime) override;
    
        void OnEnter(Pooka& pooka) override;
        void OnExit(Pooka& pooka) override;

		const float DEATH_TIME{ 0.5f }; // Time before the Pooka is removed from the scene after death to give crushed animation time to show

    private:
		float m_DeathTimer{ 0.0f };

    };
    
    class GhostState : public PookaState
    {
    public:
        PookaStates::PookaState* Update(Pooka& pooka, float deltaTime) override;
    
        void OnEnter(Pooka& pooka) override;
        void OnExit(Pooka& pooka) override;

    private:
        ColliderComponent* m_PookaCollider{};
        int m_StartIndex{};
    };    
}