#pragma once
#include <vec2.hpp>
#include <vector>
#include "GridComponent.h"
#include "Helpers.h"
#include "Command.h"

class GridComponent;
struct Cell;
class ColliderComponent;
class SpriteComponent;
class Pooka;

namespace PookaStates
{
    class PookaState 
    {
    public:
        virtual ~PookaState() {}
        virtual void Render(const Pooka&) const {}
        virtual std::unique_ptr<PookaState> Update(Pooka&, float) { return nullptr; }
    
        virtual void OnEnter(Pooka& ) {}
    	virtual void OnExit(Pooka& ) {}

        const float MOVEMENT_SPEED{ 30.f };
        const int   SNAP_DISTANCE {  1   };

    protected:
        SpriteComponent* m_Sprite{ nullptr };
        ColliderComponent* m_Collider{ nullptr };
		glm::vec2 m_Target{};
    };
    
    class MovingState : public PookaState
    {
    public:
		void Render(const Pooka&) const override;
        std::unique_ptr<PookaState> Update(Pooka& pooka, float deltaTime) override;
    
    	void OnEnter(Pooka& pooka) override;
    	void OnExit(Pooka& pooka) override;

        const float GHOST_TIMER{ 5.f };

    private:
        // Private member functions
        glm::vec2 FindBestNextTile(Pooka& pooka);
        std::vector<GridComponent::Cell> GetPossibleCells(Pooka& pooka);

        void SetDirection(Pooka& pooka);
        void MoveTowardsGoal(const Pooka& pooka, float deltaTime);

        // Private member variables
		MoveDirection m_Direction{ MoveDirection::Right };
		glm::vec2 m_CurrentTarget{};

        float m_AccumulatedTime{};

        bool m_HasReachedTarget{};

        //Commands
        std::unique_ptr<MoveCommand> m_MoveLeftUPtr;
        std::unique_ptr<MoveCommand> m_MoveRightUPtr;
        std::unique_ptr<MoveCommand> m_MoveUpUPtr;
        std::unique_ptr<MoveCommand> m_MoveDownUPtr;
    };

    class InflatedState : public PookaState
    {
    public:
        void Render(const Pooka&) const override;
        std::unique_ptr<PookaState> Update(Pooka& pooka, float deltaTime) override;
    
        void OnEnter(Pooka& pooka) override;
        void OnExit(Pooka& pooka) override;

        const float RESET_THRESHOLD{ 1.f }; // Time player has to inflate again before inflation is reset

    private:
        glm::vec2 m_StartPos{};

        float m_ResetTimer{};

		Inflated m_PreviousState{ Inflated::None }; 
    };
    
    class DeathState : public PookaState
    {
    public:
        void Render(const Pooka&) const override;
        std::unique_ptr<PookaState> Update(Pooka& pooka, float deltaTime) override;
    
        void OnEnter(Pooka& pooka) override;
        void OnExit(Pooka& pooka) override;

		const float DEATH_TIME{ 0.5f }; // Time before the Pooka is removed from the scene after death to give crushed animation time to show

    private:
		float m_DeathTimer{ 0.0f };

    };
    
    class GhostState : public PookaState
    {
    public:
        void Render(const Pooka&) const override;
        std::unique_ptr<PookaState> Update(Pooka& pooka, float deltaTime) override;
    
        void OnEnter(Pooka& pooka) override;
        void OnExit(Pooka& pooka) override;

    private:
        ColliderComponent* m_PookaCollider{};

        int m_StartIndex{};

        bool m_IsRematerialising{ false };
    };    
}