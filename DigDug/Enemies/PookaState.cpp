#include "PookaState.h"
#include <iostream>

#include "GameObject.h"
#include "Pooka.h"
#include "SpriteComponent.h"

using namespace PookaStates;
//-----------------------------------------------------
// MovingState Class
//-----------------------------------------------------

void MovingState::HandleInput(Pooka&) 
{

}

PookaStates::PookaState* MovingState::Update(Pooka& , float deltaTime)
{
    m_AccTime += deltaTime;

	if (m_AccTime >= 2.f) return &PookaState::inflating;

	return nullptr;
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

PookaStates::PookaState* InflatedState::Update(Pooka& , float deltaTime)
{
    m_AccTime += deltaTime;

	if (m_AccTime >= 2.f) return &PookaState::dying;

    return nullptr;
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

PookaStates::PookaState* DeathState::Update(Pooka& , float deltaTime)
{
    m_AccTime += deltaTime;

	if (m_AccTime >= 2.f) return &PookaState::ghosting;

    return nullptr;
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

PookaStates::PookaState* GhostState::Update(Pooka& , float deltaTime)
{
    m_AccTime += deltaTime;

	if (m_AccTime >= 2.f) return &PookaState::fleeing;

    return nullptr;
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

PookaStates::PookaState* FleeingState::Update(Pooka& , float deltaTime)
{
    m_AccTime += deltaTime;

    if (m_AccTime >= 2.f) return &PookaState::moving;

	return nullptr;
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