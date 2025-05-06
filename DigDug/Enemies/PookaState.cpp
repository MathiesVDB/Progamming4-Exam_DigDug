#include "PookaState.h"
#include <iostream>

#include "Pooka.h"

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
    else
    {
        std::cout << "Current state = moving\n";
    }
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
    else
    {
        std::cout << "Current state = inflating\n";
    }
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
    else
    {
        std::cout << "Current state = dying\n";
    }
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
        pooka.SetState(&PookaState::moving);
        m_AccTime = 0.f;
    }
    else
    {
        std::cout << "Current state = ghosting\n";
    }
}

//-----------------------------------------------------
// Statics
//-----------------------------------------------------

MovingState     PookaState::moving;
InflatedState   PookaState::inflating;
DeathState      PookaState::dying;
GhostState      PookaState::ghosting;