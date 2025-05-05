#pragma once
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
    virtual void Update(Pooka& , float ) {};
};

class MovingState : public PookaState
{
    void HandleInput(Pooka& ) override
    {

    }

    void Update(Pooka& , float ) override
    {

    }
};

class InflatedState : public PookaState
{
    void HandleInput(Pooka& ) override
    {

    }

    void Update(Pooka& , float ) override
    {

    }
};

class DeathState : public PookaState
{
    void HandleInput(Pooka& ) override
    {

    }

    void Update(Pooka& , float ) override
    {

    }
};

class GhostState : public PookaState
{
    void HandleInput(Pooka& ) override
    {

    }

    void Update(Pooka& , float ) override
    {

    }
};