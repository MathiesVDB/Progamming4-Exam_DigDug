#pragma once

class SpriteComponent;
struct CollisionEvent;
class Rock;

namespace RockStates
{
    class IdleState;
    class TiltState;
    class FallState;
    class BreakState;

	class RockState
    {
    public:
        static IdleState        idling;
        static TiltState        tilting;
        static FallState        falling;
        static BreakState       breaking;

        virtual ~RockState() {}
        virtual RockStates::RockState* HandleCollision(Rock&, const CollisionEvent&) { return nullptr; }
        virtual RockStates::RockState* Update(Rock&, float) { return nullptr; }

        virtual void OnEnter(Rock&) {}
        virtual void OnExit(Rock&) {}

	protected:
        SpriteComponent* m_Sprite{ nullptr };
    };

    class IdleState : public RockState
    {
	public:
        RockStates::RockState* HandleCollision(Rock&, const CollisionEvent& collision) override;
		RockStates::RockState* Update(Rock& rock, float deltaTime) override;

		void OnEnter(Rock& rock) override;
		void OnExit(Rock& rock) override;
    };

    class TiltState : public RockState
    {
    public:
        RockStates::RockState* HandleCollision(Rock&, const CollisionEvent& collision) override;
        RockStates::RockState* Update(Rock& rock, float deltaTime) override;

        void OnEnter(Rock& rock) override;
        void OnExit(Rock& rock) override;
    };

    class FallState : public RockState
    {
    public:
        RockStates::RockState* HandleCollision(Rock&, const CollisionEvent& collision) override;
        RockStates::RockState* Update(Rock& rock, float deltaTime) override;

        void OnEnter(Rock& rock) override;
        void OnExit(Rock& rock) override;

		constexpr static float GRAVITY{ 200.f };
    };

    class BreakState : public RockState
    {
    public:
        RockStates::RockState* HandleCollision(Rock&, const CollisionEvent& collision) override;
        RockStates::RockState* Update(Rock& rock, float deltaTime) override;

        void OnEnter(Rock& rock) override;
        void OnExit(Rock& rock) override;
    };
}