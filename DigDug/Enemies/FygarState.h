#pragma once
#include <memory>
#include <vec2.hpp>
#include <vector>
#include "Helpers.h"
#include "GridComponent.h"
#include "Command.h"

class ColliderComponent;
class SpriteComponent;
class Fygar;

namespace FygarStates
{
    class FygarState
    {
    public:
        virtual ~FygarState() {}
        virtual std::unique_ptr<FygarState> Update(Fygar&, float) { return nullptr; }

        virtual void OnEnter(Fygar&) {}
        virtual void OnExit(Fygar&) {}

        static constexpr float MOVEMENT_SPEED   { 30.f };
        static constexpr int   SNAP_DISTANCE    {  1 };

    protected:
        SpriteComponent* m_Sprite{ nullptr };
        ColliderComponent* m_Collider{ nullptr };
        glm::vec2 m_Target{};
    };

    class MovingState : public FygarState
    {
    public:
        std::unique_ptr<FygarState> Update(Fygar& pooka, float deltaTime) override;

        void OnEnter(Fygar& pooka) override;
        void OnExit(Fygar& pooka) override;

        const float GHOST_TIMER{ 7.f };

    private:
        // Private member functions
        glm::vec2 FindBestNextTile(Fygar& pooka);
        std::vector<GridComponent::Cell> GetPossibleCells(Fygar& pooka);

        void SetDirection(Fygar& fygar);
        void MoveTowardsGoal(const Fygar& fygar, float deltaTime);

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

    class InflatedState : public FygarState
    {
    public:
        std::unique_ptr<FygarState> Update(Fygar& fygar, float deltaTime) override;

        void OnEnter(Fygar& fygar) override;
        void OnExit(Fygar& fygar) override;

        const float RESET_THRESHOLD{ 1.f }; // Time player has to inflate again before inflation is reset

    private:
        glm::vec2 m_StartPos{};

        float m_ResetTimer{};

        Inflated m_PreviousState{ Inflated::None };
    };

    class DeathState : public FygarState
    {
    public:
        std::unique_ptr<FygarState> Update(Fygar& fygar, float deltaTime) override;

        void OnEnter(Fygar& fygar) override;
        void OnExit(Fygar& fygar) override;

        const float DEATH_TIME{ 0.5f }; // Time before the Pooka is removed from the scene after death to give crushed animation time to show

    private:
        float m_DeathTimer{ 0.0f };

    };

    class GhostState : public FygarState
    {
    public:
        std::unique_ptr<FygarState> Update(Fygar& fygar, float deltaTime) override;

        void OnEnter(Fygar& fygar) override;
        void OnExit(Fygar& fygar) override;

    private:
        ColliderComponent* m_FygarCollider{};

        int m_StartIndex{};

        bool m_IsRematerialising{ false };
    };

    class AttackState : public FygarState
    {
    public:
        std::unique_ptr<FygarState> Update(Fygar& fygar, float deltaTime) override;

        void OnEnter(Fygar& fygar) override;
        void OnExit(Fygar& fygar) override;

        const float ATTACK_TIME{ 2.f };

    private:
        void CreateFireGameObject(Fygar& fygar);

        dae::GameObject* m_AttackObject{ nullptr };
        SpriteComponent* m_AttackSprite{ nullptr };

        float m_AccumulatedTime{};
    };

}