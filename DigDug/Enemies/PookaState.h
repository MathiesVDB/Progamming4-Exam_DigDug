#pragma once
#include <iostream>
#include <memory>
#include "Pooka.h"

class MovingState;
class InflatedState;
class DeathState;
class GhostState;

class PookaState 
{
public:
    static MovingState      moving;
    static InflatedState    inflating;
    static DeathState       dying;
    static GhostState       ghosting;

    virtual ~PookaState() {}
    virtual void HandleInput(Pooka& ) {};
    virtual void Update(Pooka& pooka, float deltaTime) {};

protected:
    float m_AccTime{};
};

class MovingState : public PookaState
{
    void HandleInput(Pooka& ) override
    {

    }

    void Update(Pooka& pooka, float deltaTime) override
    {
        m_AccTime += deltaTime;

        if (m_AccTime >= 2.f)
        {
            pooka.m_State = &PookaState::inflating;
        }
        else
        {
            std::cout << "Current state = moving\n";
        }
    }
};

class InflatedState : public PookaState
{
    void HandleInput(Pooka& ) override
    {

    }

    void Update(Pooka& pooka, float deltaTime) override
    {
        m_AccTime += deltaTime;

        if (m_AccTime >= 2.f)
        {
            pooka.m_State = &PookaState::dying;
        }
        else
        {
            std::cout << "Current state = inflating\n";
        }
    }
};

class DeathState : public PookaState
{
    void HandleInput(Pooka& ) override
    {

    }

    void Update(Pooka& pooka, float deltaTime) override
    {
        m_AccTime += deltaTime;

        if (m_AccTime >= 2.f)
        {
            pooka.m_State = &PookaState::ghosting;
        }
        else
        {
            std::cout << "Current state = dying\n";
        }
    }
};

class GhostState : public PookaState
{
    void HandleInput(Pooka& ) override
    {

    }

    void Update(Pooka& pooka, float deltaTime) override
    {
        m_AccTime += deltaTime;

        if (m_AccTime >= 2.f)
        {
            pooka.m_State = &PookaState::moving;
        }
        else
        {
            std::cout << "Current state = ghosting\n";
        }
    }
};