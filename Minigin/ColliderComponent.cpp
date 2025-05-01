//---------------------------
// Include Files
//---------------------------
#include "ColliderComponent.h"

//---------------------------
// Constructor & Destructor
//---------------------------
ColliderComponent::ColliderComponent(dae::GameObject* owner)
	: Component(owner)
{
	// nothing to create
}

ColliderComponent::~ColliderComponent()
{
	// nothing to destroy
}

bool ColliderComponent::IsOverlapping(const ColliderComponent& other) const
{
	// Check if the bounding boxes of the two colliders overlap
	return (m_BoundingBox.x						< other.m_BoundingBox.x + other.m_BoundingBox.w &&
			m_BoundingBox.x + m_BoundingBox.w	> other.m_BoundingBox.x &&
			m_BoundingBox.y + m_BoundingBox.h	< other.m_BoundingBox.y &&
			m_BoundingBox.y						> other.m_BoundingBox.y + other.m_BoundingBox.h);
}