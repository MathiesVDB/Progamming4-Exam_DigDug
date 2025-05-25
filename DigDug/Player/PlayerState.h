#pragma once
#include "Helpers.h"

class SpriteComponent;
class Player;

namespace PlayerStates
{
    class IdleState;
    class MovingState;
    class DiggingState;
    class DeathState;
    class AttackState;
    
    class PlayerState
    {
    public:
        static IdleState    idling;
        static MovingState  moving;
        static DiggingState digging;
        static DeathState   dying;
        static AttackState  attacking;
    
        virtual ~PlayerState() {}
        virtual void Render(const Player&) const {}
        virtual PlayerStates::PlayerState* Update(Player&, float) { return nullptr; }
    
        virtual void OnEnter(Player&) {}
        virtual void OnExit(Player&) {}
    };
    
    class IdleState : public PlayerState
    {
    public:
        void Render(const Player& player) const override;
        PlayerStates::PlayerState* Update(Player& player, float deltaTime) override;
    
        void OnEnter(Player& player) override;
        void OnExit(Player& player) override;
    };
    
    class MovingState : public PlayerState
    {
    public:
        void Render(const Player& player) const override;
        PlayerStates::PlayerState* Update(Player& player, float deltaTime) override;
    
        void OnEnter(Player& player) override;
        void OnExit(Player& player) override;
    };
    
    class DiggingState : public PlayerState
    {
    public:
        void Render(const Player& player) const override;
        PlayerStates::PlayerState* Update(Player& player, float deltaTime) override;
    
        void OnEnter(Player& player) override;
        void OnExit(Player& player) override;

    private:
        void DigCurrentTile(Player& player) const;

		MoveDirection m_PreviousDirection{ MoveDirection::Right };
        Point2f m_Position{};
    };
    
    class DeathState : public PlayerState
    {
    public:
        void Render(const Player& player) const override;
        PlayerStates::PlayerState* Update(Player& player, float deltaTime) override;
    
        void OnEnter(Player& player) override;
        void OnExit(Player& player) override;
    };
    
    class AttackState : public PlayerState
    {
    public:
        void Render(const Player& player) const override;
        PlayerStates::PlayerState* Update(Player& player, float deltaTime) override;
    
        void OnEnter(Player& player) override;
        void OnExit(Player& player) override;
    };
}