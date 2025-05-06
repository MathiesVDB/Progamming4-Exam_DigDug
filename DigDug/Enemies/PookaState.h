#pragma once

class MovingState;
class InflatedState;
class DeathState;
class GhostState;


class Pooka;
class PookaState 
{
public:
    static MovingState      moving;
    static InflatedState    inflating;
    static DeathState       dying;
    static GhostState       ghosting;

    virtual ~PookaState() {}
    virtual void HandleInput(Pooka& ) {};
    virtual void Update(Pooka&, float ) {};

protected:
    float m_AccTime{};
};

class MovingState : public PookaState
{
public:
	void HandleInput(Pooka&) override;
	void Update(Pooka& pooka, float deltaTime) override;
};

class InflatedState : public PookaState
{
public:
	void HandleInput(Pooka&) override;
	void Update(Pooka& pooka, float deltaTime) override;
};

class DeathState : public PookaState
{
public:
	void HandleInput(Pooka&) override;
	void Update(Pooka& pooka, float deltaTime) override;
};

class GhostState : public PookaState
{
public:
    void HandleInput(Pooka&) override;
    void Update(Pooka& pooka, float deltaTime) override;
};