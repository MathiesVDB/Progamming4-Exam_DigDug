#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include <vector>
#include "ColliderComponent.h"
#include "Observer.h"

struct CollisionEvent
{
	dae::GameObject* sender;  // Collider
	dae::GameObject* receiver; // Collided
};

class CollisionSystem : public dae::Subject
{
public:
	virtual ~CollisionSystem() = default;

	virtual void RegisterCollider(ColliderComponent* collider) = 0;
	virtual void UnregisterCollider(ColliderComponent* collider) = 0;
	virtual void CheckCollisions() = 0;

	virtual const CollisionEvent& GetLastCollisionEvent() const = 0;

protected:
	CollisionEvent m_LastCollisionEvent{};
};

class RealCollisionSystem final : public CollisionSystem
{
public:
	RealCollisionSystem(); // Constructor
	~RealCollisionSystem() override; // Destructor

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	RealCollisionSystem(const RealCollisionSystem& other)					= default;
	RealCollisionSystem(RealCollisionSystem&& other) noexcept				= default;
	RealCollisionSystem& operator=(const RealCollisionSystem& other)		= default;
	RealCollisionSystem& operator=(RealCollisionSystem&& other) noexcept	= default;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	void RegisterCollider(ColliderComponent* collider) override;
	void UnregisterCollider(ColliderComponent* collider) override;
	void CheckCollisions() override;

	const CollisionEvent& GetLastCollisionEvent() const override;

private:
	//-------------------------------------------------
	// Private member functions								
	//-------------------------------------------------
	bool IsOverlapping(const SDL_Rect& rectA, const SDL_Rect& rectB) const;

	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	std::vector<ColliderComponent*> m_Colliders;
};

class NullCollisionSystem : public CollisionSystem
{
public:
	void RegisterCollider(ColliderComponent*) override { std::cout << "NULL COLLISION SYSTEM\n"; }
	void UnregisterCollider(ColliderComponent*) override { std::cout << "NULL COLLISION SYSTEM\n"; }
	void CheckCollisions() override { std::cout << "NULL COLLISION SYSTEM\n"; }

	virtual const CollisionEvent& GetLastCollisionEvent() const override { return m_LastCollisionEvent; };
};
