#pragma once
#include <vec2.hpp>
#include <vector>

#include "Component.h"
#include "Helpers.h"

struct CollisionEvent;

namespace dae
{
	class GameObject;
}

class ColliderComponent;

class RopeComponent : public dae::Component
{
public:
	RopeComponent(dae::GameObject* owner, dae::GameObject* head, dae::GameObject* middle, dae::GameObject* tail);

	void Update(float deltaTime);
	void HandleCollision(const CollisionEvent& collision);

	void ActivateRope(const glm::vec2& startPos, MoveDirection shootDirection);
	void ChangeRopeTexture(MoveDirection shootDirection) const;

	bool GetRopeStatus() const { return m_Active; }

	void ToggleAttacking() { m_IsAttacking = !m_IsAttacking; }

	static constexpr float ROPE_SPEED{ 200.f };

private:
	MoveDirection m_Direction{ MoveDirection::Right };

	dae::GameObject* m_RopeHead;
	dae::GameObject* m_RopeMiddle;
	dae::GameObject* m_RopeTail;

	dae::GameObject* m_HitTarget{ nullptr };

	ColliderComponent* m_RopeCollider;

	glm::vec2 m_StartPos;

	bool m_Active;
	bool m_IsAttacking;
	bool m_HasHit;

	float m_Width;
	float m_ElapsedDistance;
};