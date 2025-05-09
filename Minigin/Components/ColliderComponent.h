#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include <SDL_rect.h>
#include "Component.h"
#include <iostream>

//-----------------------------------------------------
// ColliderComponent Class									 
//-----------------------------------------------------
enum Tag
{
	GROUND,
	FRIENDLY_ENTITY,
	ENEMY_ENTITY
};

class ColliderComponent final : public dae::Component
{
public:
	ColliderComponent(dae::GameObject* owner, Tag tag);

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	void Update(float deltaTime) override;
	void Render() const override;

	SDL_Rect GetBoundingBox() const { return m_BoundingBox; }
	Tag GetTag() const { return m_Tag; }

	bool m_WasHit;

private:
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	SDL_Rect m_BoundingBox;
	Tag m_Tag;
	int m_Width;
	int m_Height;
};
