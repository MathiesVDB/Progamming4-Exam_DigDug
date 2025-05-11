#pragma once

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
        virtual void Update(Player&, float) {}
    
        virtual void OnEnter(Player&) {}
        virtual void OnExit(Player&) {}
    };
    
    class IdleState : public PlayerState
    {
    public:
        void Render(const Player& player) const override;
        void Update(Player& player, float deltaTime) override;
    
        void OnEnter(Player& player) override;
        void OnExit(Player& player) override;
    };
    
    class MovingState : public PlayerState
    {
    public:
        void Render(const Player& player) const override;
        void Update(Player& player, float deltaTime) override;
    
        void OnEnter(Player& player) override;
        void OnExit(Player& player) override;
    };
    
    class DiggingState : public PlayerState
    {
    public:
        void Render(const Player& player) const override;
        void Update(Player& player, float deltaTime) override;
    
        void OnEnter(Player& player) override;
        void OnExit(Player& player) override;
    };
    
    class DeathState : public PlayerState
    {
    public:
        void Render(const Player& player) const override;
        void Update(Player& player, float deltaTime) override;
    
        void OnEnter(Player& player) override;
        void OnExit(Player& player) override;
    };
    
    class AttackState : public PlayerState
    {
    public:
        void Render(const Player& player) const override;
        void Update(Player& player, float deltaTime) override;
    
        void OnEnter(Player& player) override;
        void OnExit(Player& player) override;
    };
}