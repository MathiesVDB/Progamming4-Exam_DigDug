#pragma once
#include <vec2.hpp>
#include "Helpers.h"

namespace dae
{
	class GameObject;
}

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

    protected:
		SpriteComponent* m_Sprite{ nullptr };
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
        void DigCurrentTile(Player& player);

        bool CheckHasBeenDug(Player& player, dae::GameObject* coverTile, int index);
        bool IsTileMoveAllowed(MoveDirection direction, const glm::vec2& currentTilePos, const glm::vec2& playerPos);

		MoveDirection m_PreviousDirection{ MoveDirection::Right };
        glm::vec2 m_Position{};
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

    private:
        void SetPlayerThrow(Player& player) const;
        void SetPlayerPump(Player& player);

        bool m_IsPumping{ false };
        bool m_InitializedPump{ false };
        int m_StartPumpFrame{ 0 };
    };
}