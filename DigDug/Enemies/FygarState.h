#pragma once
#include <vec2.hpp>

#include "Helpers.h"

class GridComponent;
class ColliderComponent;
class SpriteComponent;
class Fygar;

namespace FygarStates
{
    class MovingState;
    class InflatedState;
    class DeathState;
    class GhostState;
    class AttackState;

    class FygarState
    {
    public:
        static MovingState      moving;
        static InflatedState    inflating;
        static DeathState       dying;
        static GhostState       ghosting;
        static AttackState      attacking;

        virtual ~FygarState() {}
        virtual FygarStates::FygarState* Update(Fygar&, float) { return nullptr; }

        virtual void OnEnter(Fygar&) {}
        virtual void OnExit(Fygar&) {}

    protected:
        SpriteComponent* m_Sprite{ nullptr };
        glm::vec2 m_Target{};
    };

    class MovingState : public FygarState
    {
    public:
        FygarStates::FygarState* Update(Fygar& fygar, float deltaTime) override;

        void OnEnter(Fygar& pooka) override;
        void OnExit(Fygar& pooka) override;

        const float MOVEMENT_SPEED{ 1000.f };

    private:
        // Private member functions
        glm::vec2 GetNextStepToward(const glm::vec2& currentPos, const glm::vec2& targetPos, GridComponent* grid);

        // Private member variables
        bool m_HasReachedTarget{};
    };

    class InflatedState : public FygarState
    {
    public:
        FygarStates::FygarState* Update(Fygar& fygar, float deltaTime) override;

        void OnEnter(Fygar& fygar) override;
        void OnExit(Fygar& pooka) override;

        const float RESET_THRESHOLD{ 1.f }; // Time player has to inflate again before inflation is reset

    private:
        float m_ResetTimer{};

        Inflated m_PreviousState{ Inflated::None };
    };

    class DeathState : public FygarState
    {
    public:
        FygarStates::FygarState* Update(Fygar& fygar, float deltaTime) override;

        void OnEnter(Fygar& fygar) override;
        void OnExit(Fygar& pooka) override;

        const float DEATH_TIME{ 0.5f }; // Time before the Pooka is removed from the scene after death to give crushed animation time to show

    private:
        float m_DeathTimer{ 0.0f };

    };

    class GhostState : public FygarState
    {
    public:
        FygarStates::FygarState* Update(Fygar& fygar, float deltaTime) override;

        void OnEnter(Fygar& fygar) override;
        void OnExit(Fygar& pooka) override;

        const float MOVEMENT_SPEED{ 1000.f };

    private:
        ColliderComponent* m_PookaCollider{};
    };

    class AttackState : public FygarState
    {
    public:
        FygarStates::FygarState* Update(Fygar& pooka, float deltaTime) override;

        void OnEnter(Fygar& pooka) override;
        void OnExit(Fygar& pooka) override;
    };

}