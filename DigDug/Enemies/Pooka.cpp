#include "Pooka.h"

#include "SpriteComponent.h"
#include "GameObject.h"

Pooka::Pooka(dae::GameObject* owner)
	:	Component(owner)
{
	m_State = &PookaState::moving;
}

void Pooka::HandleInput()
{
	m_State->HandleInput(*this);
}

void Pooka::Update(float deltaTime)
{
	m_State->Update(*this, deltaTime);
}

void Pooka::Render() const
{
	Component::Render();
}

void Pooka::SetState(PookaState* state)
{
	m_State = state;
	UpdateSpriteForCurrentState();
}

void Pooka::UpdateSpriteForCurrentState()
{
	auto sprite = GetOwner()->GetComponent<SpriteComponent>();
    if (!sprite) return;

    if (m_State == &PookaState::moving)
    {
        sprite->SetNewTexture("Sprites/Pooka/DefaultSprite.bmp", 2, 5, 0, 1);
    }
    else if (m_State == &PookaState::inflating)
    {
        sprite->SetNewTexture("Sprites/Pooka/InflateRightSprite.bmp", 1, 4, 0, 3);
    }
    else if (m_State == &PookaState::dying)
    {
        sprite->SetNewTexture("Sprites/Pooka/DefaultSprite.bmp", 2, 5, 2, 2);
    }
    else if (m_State == &PookaState::ghosting)
    {
        sprite->SetNewTexture("Sprites/Pooka/DefaultSprite.bmp", 2, 5, 3, 4);
    }
}
