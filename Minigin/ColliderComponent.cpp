//---------------------------
// Include Files
//---------------------------
#include "ColliderComponent.h"

//---------------------------
// Constructor & Destructor
//---------------------------
ColliderComponent::ColliderComponent(dae::GameObject* owner)
	:	Component(owner)
{
	// nothing to create
}

bool ColliderComponent::IsOverlapping(const SDL_Rect& otherBoundingBox) const
{
	return (m_BoundingBox.x						< otherBoundingBox.x + otherBoundingBox.w	&&
			m_BoundingBox.x + m_BoundingBox.w	> otherBoundingBox.x						&&
			m_BoundingBox.y + m_BoundingBox.h	< otherBoundingBox.y						&&
			m_BoundingBox.y						> otherBoundingBox.y + otherBoundingBox.h);
}

