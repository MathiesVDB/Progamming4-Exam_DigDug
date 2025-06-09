#pragma once
#include <memory>
#include <vector>

#include "Helpers.h"

namespace dae
{
	class GameObject;
}

class ColliderComponent;
class SpriteComponent;
struct CollisionEvent;
class Rock;

namespace RockStates
{
	class RockState
    {
    public:
        virtual ~RockState() {}
        virtual std::unique_ptr<RockStates::RockState> HandleCollision(Rock&, const CollisionEvent&) { return nullptr; }
        virtual std::unique_ptr<RockStates::RockState> Update(Rock&, float) { return nullptr; }

        virtual void OnEnter(Rock&) {}
        virtual void OnExit(Rock&) {}

	protected:
        SpriteComponent* m_Sprite{ nullptr };
    };

    class IdleState : public RockState
    {
	public:
        std::unique_ptr<RockStates::RockState> HandleCollision(Rock&, const CollisionEvent& collision) override;
		std::unique_ptr<RockStates::RockState> Update(Rock& rock, float deltaTime) override;

		void OnEnter(Rock& rock) override;
		void OnExit(Rock& rock) override;
    };

    class TiltState : public RockState
    {
    public:
        std::unique_ptr<RockStates::RockState> HandleCollision(Rock&, const CollisionEvent& collision) override;
        std::unique_ptr<RockStates::RockState> Update(Rock& rock, float deltaTime) override;

        void OnEnter(Rock& rock) override;
        void OnExit(Rock& rock) override;

    private:
        std::vector<dae::GameObject*>    m_Players{nullptr};
    };

    class FallState : public RockState
    {
    public:
        std::unique_ptr<RockStates::RockState> HandleCollision(Rock& rock, const CollisionEvent& collision) override;
        std::unique_ptr<RockStates::RockState> Update(Rock& rock, float deltaTime) override;

        void OnEnter(Rock& rock) override;
        void OnExit(Rock& rock) override;

		constexpr static float GRAVITY{ 10000.f };
    };

    class BreakState : public RockState
    {
    public:
        std::unique_ptr<RockStates::RockState> HandleCollision(Rock&, const CollisionEvent& collision) override;
        std::unique_ptr<RockStates::RockState> Update(Rock& rock, float deltaTime) override;

        void OnEnter(Rock& rock) override;
        void OnExit(Rock& rock) override;
    };
}