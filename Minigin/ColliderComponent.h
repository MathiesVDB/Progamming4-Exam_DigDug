#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include <SDL_rect.h>
#include "Component.h"
#include "Helpers.h"

//-----------------------------------------------------
// ColliderComponent Class									 
//-----------------------------------------------------
class ColliderComponent final : public dae::Component
{
public:
	ColliderComponent(dae::GameObject* owner);

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	ColliderComponent(const ColliderComponent& other) = default;
	ColliderComponent(ColliderComponent&& other) noexcept = default;
	ColliderComponent& operator=(const ColliderComponent& other) = default;
	ColliderComponent& operator=(ColliderComponent&& other) noexcept = default;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	bool IsOverlapping(const SDL_Rect& otherBoundingBox) const;

private:
	//-------------------------------------------------
	// Member functions								
	//-------------------------------------------------

	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	SDL_Rect m_BoundingBox;
};
