#include "PookaState.h"
#include <iostream>

#include "GameObject.h"
#include "Pooka.h"
#include "SpriteComponent.h"

//-----------------------------------------------------
// MovingState Class
//-----------------------------------------------------

void MovingState::HandleInput(Pooka&) 
{

}

void MovingState::Update(Pooka& pooka, float deltaTime) 
{
    m_AccTime += deltaTime;

    if (m_AccTime >= 2.f)
    {
        pooka.SetState(&PookaState::inflating);
		m_AccTime = 0.f;
    }
}

void MovingState::OnEnter(Pooka& pooka)
{
    std::cout << "Current state = moving\n";
	pooka.GetOwner()->GetComponent<SpriteComponent>()->SetNewTexture("Sprites/Pooka/PookaDefaultSprite.png", 2, 5, 0, 1);
}

void MovingState::OnExit(Pooka&)
{
    m_AccTime = 0.f;
}

//-----------------------------------------------------
// InflatedState Class
//-----------------------------------------------------

void InflatedState::HandleInput(Pooka&) 
{

}

void InflatedState::Update(Pooka& pooka, float deltaTime) 
{
    m_AccTime += deltaTime;

    if (m_AccTime >= 2.f)
    {
        pooka.SetState(&PookaState::dying);
		m_AccTime = 0.f;
    }
}

void InflatedState::OnEnter(Pooka& pooka)
{
    std::cout << "Current state = inflating\n";
    pooka.GetOwner()->GetComponent<SpriteComponent>()->SetNewTexture("Sprites/Pooka/PookaInflateRightSprite.png", 1, 4, 0, 3);
}

void InflatedState::OnExit(Pooka&)
{
    m_AccTime = 0.f;
}

//-----------------------------------------------------
// DeathState Class
//-----------------------------------------------------

void DeathState::HandleInput(Pooka&) 
{

}

void DeathState::Update(Pooka& pooka, float deltaTime) 
{
    m_AccTime += deltaTime;

    if (m_AccTime >= 2.f)
    {
        pooka.SetState(&PookaState::ghosting);
        m_AccTime = 0.f;
    }
}

void DeathState::OnEnter(Pooka& pooka)
{
    std::cout << "Current state = dying\n";
    pooka.GetOwner()->GetComponent<SpriteComponent>()->SetNewTexture("Sprites/Pooka/PookaDefaultSprite.png", 2, 5, 2, 2);
}

void DeathState::OnExit(Pooka&)
{
    m_AccTime = 0.f;
}

//-----------------------------------------------------
// GhostState Class
//-----------------------------------------------------

void GhostState::HandleInput(Pooka&) 
{

}

void GhostState::Update(Pooka& pooka, float deltaTime)
{
    m_AccTime += deltaTime;

    if (m_AccTime >= 2.f)
    {
        pooka.SetState(&PookaState::fleeing);
        m_AccTime = 0.f;
    }
}

void GhostState::OnEnter(Pooka& pooka)
{
    std::cout << "Current state = ghosting\n";
    pooka.GetOwner()->GetComponent<SpriteComponent>()->SetNewTexture("Sprites/Pooka/PookaDefaultSprite.png", 2, 5, 3, 4);
}

void GhostState::OnExit(Pooka&)
{
    m_AccTime = 0.f;
}

//-----------------------------------------------------
// FleeingState Class
//-----------------------------------------------------

void FleeingState::HandleInput(Pooka& )
{
}

void FleeingState::Update(Pooka& pooka, float deltaTime)
{
    m_AccTime += deltaTime;

    if (m_AccTime >= 2.f)
    {
        pooka.SetState(&PookaState::moving);
        m_AccTime = 0.f;
    }
}

void FleeingState::OnEnter(Pooka& pooka)
{
    std::cout << "Current state = ghosting\n";
    pooka.GetOwner()->GetComponent<SpriteComponent>()->SetNewTexture("Sprites/Pooka/PookaDefaultSprite.png", 2, 5, 5, 6);
}

void FleeingState::OnExit(Pooka&)
{
	m_AccTime = 0.f;
}

//-----------------------------------------------------
// Statics
//-----------------------------------------------------

MovingState     PookaState::moving;
InflatedState   PookaState::inflating;
DeathState      PookaState::dying;
GhostState      PookaState::ghosting;
FleeingState    PookaState::fleeing;