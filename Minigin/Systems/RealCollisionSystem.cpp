//---------------------------
// Include Files
//---------------------------
#include "RealCollisionSystem.h"
#include "Observer.h"
#include "CollisionHandler.h"

//---------------------------
// Constructor & Destructor
//---------------------------
RealCollisionSystem::RealCollisionSystem()
{
	std::cout << "[Collision System] Collision system initialized.\n";
}

RealCollisionSystem::~RealCollisionSystem()
{
	// nothing to destroy
}

void RealCollisionSystem::RegisterCollider(ColliderComponent* collider)
{
	if (collider)
	{
		m_Colliders.emplace_back(collider);
	}
	else
	{
		std::cerr << "Collider is null, cannot register.\n";
	}
}

void RealCollisionSystem::UnregisterCollider(ColliderComponent* collider)
{
	auto iterator = std::find(m_Colliders.begin(), m_Colliders.end(), collider);
	if (iterator != m_Colliders.end())
	{
		m_Colliders.erase(iterator);
	}
}

void RealCollisionSystem::CheckCollisions()
{
    for (size_t iterator = 0; iterator < m_Colliders.size(); ++iterator) 
    {
        for (size_t counter = iterator + 1; counter < m_Colliders.size(); ++counter) 
        {
            ColliderComponent* colliderA = m_Colliders[iterator];
            ColliderComponent* colliderB = m_Colliders[counter];

			colliderA->m_WasHit = false;
			colliderB->m_WasHit = false;

			Tag tagA = colliderA->GetTag();
			Tag tagB = colliderB->GetTag();

			if ((tagA == GROUND				&& tagB == GROUND)			||
				(tagA == FRIENDLY_ENTITY	&& tagB == FRIENDLY_ENTITY) ||
				(tagA == ENEMY_ENTITY		&& tagB == ENEMY_ENTITY))
			{
				continue;
			}

            if (IsOverlapping(colliderA->GetBoundingBox(), colliderB->GetBoundingBox())) 
            {
				colliderA->m_WasHit = true;

				m_LastCollisionEvent = { colliderA->GetOwner(), colliderB->GetOwner() };
				Notify(colliderA->GetOwner(), dae::EventRegistry::GetInstance().GetEventID("CollisionEvent"));
				Notify(colliderB->GetOwner(), dae::EventRegistry::GetInstance().GetEventID("CollisionEvent"));
            }
        }
    }
}

const CollisionEvent& RealCollisionSystem::GetLastCollisionEvent() const
{
	return m_LastCollisionEvent;
}

bool RealCollisionSystem::IsOverlapping(const SDL_Rect& callingBoundingBox, const SDL_Rect& otherBoundingBox) const
{
	return (callingBoundingBox.x						< otherBoundingBox.x + otherBoundingBox.w &&
			callingBoundingBox.x + callingBoundingBox.w > otherBoundingBox.x &&
			callingBoundingBox.y						< otherBoundingBox.y + otherBoundingBox.h &&
			callingBoundingBox.y + callingBoundingBox.h > otherBoundingBox.y);
}

