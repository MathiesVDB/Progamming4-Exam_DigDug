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

void MovingState::OnEnter()
{
	
}

void MovingState::OnExit()
{
	
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

void InflatedState::OnEnter()
{

}

void InflatedState::OnExit()
{

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

void DeathState::OnEnter()
{

}

void DeathState::OnExit()
{

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
    else
    {
        std::cout << "Current state = ghosting\n";
    }
}

void GhostState::OnEnter()
{

}

void GhostState::OnExit()
{

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
    else
    {
        std::cout << "Current state = ghosting\n";
    }
}

void FleeingState::OnEnter()
{
    PookaState::OnEnter();
}

void FleeingState::OnExit()
{
    PookaState::OnExit();
}

//-----------------------------------------------------
// Statics
//-----------------------------------------------------

MovingState     PookaState::moving;
InflatedState   PookaState::inflating;
DeathState      PookaState::dying;
GhostState      PookaState::ghosting;