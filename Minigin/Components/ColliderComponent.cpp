//---------------------------
// Include Files
//---------------------------
#include "ColliderComponent.h"

#include "GameObject.h"
#include "Renderer.h"
#include "ServiceLocator.h"
#include "SpriteComponent.h"
#include "TextureComponent.h"

//---------------------------
// Constructor & Destructor
//---------------------------
ColliderComponent::ColliderComponent(dae::GameObject* owner, Tag tag)
	:	Component(owner),
		m_Tag{ tag }
{
	ServiceLocator::GetCollisionSystem().RegisterCollider(this);

	if (owner->HasComponent<TextureComponent>())
	{
		m_Width  = owner->GetComponent<TextureComponent>()->GetWidth();
		m_Height = owner->GetComponent<TextureComponent>()->GetHeight();
	}
	else if (owner->HasComponent<SpriteComponent>())
	{
		m_Width  = static_cast<int>(owner->GetComponent<SpriteComponent>()->GetSpriteSize().x);
		m_Height = static_cast<int>(owner->GetComponent<SpriteComponent>()->GetSpriteSize().y);
	}
	else
	{
		std::cerr << "ColliderComponent requires a TextureComponent or SpriteComponent to be attached to the same GameObject.\n";
		return;
	}

	m_BoundingBox = { static_cast<int>(owner->GetWorldPosition().x), static_cast<int>(owner->GetWorldPosition().y), m_Width, m_Height };
}

void ColliderComponent::Update(float )
{
	m_BoundingBox.x = static_cast<int>(GetOwner()->GetWorldPosition().x);
	m_BoundingBox.y = static_cast<int>(GetOwner()->GetWorldPosition().y);
}

void ColliderComponent::Render() const
{
#if  _DEBUG
	SDL_Renderer* sdlRenderer = dae::Renderer::GetInstance().GetSDLRenderer();

	if (m_WasHit)
		SDL_SetRenderDrawColor(sdlRenderer, 255, 0, 0, 255);
	else
		SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);

	SDL_RenderDrawRect(sdlRenderer, &m_BoundingBox);
#endif
}