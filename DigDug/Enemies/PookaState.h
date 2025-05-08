#pragma once

class MovingState;
class InflatedState;
class DeathState;
class GhostState;
class FleeingState;

class Pooka;
class PookaState 
{
public:
    static MovingState      moving;
    static InflatedState    inflating;
    static DeathState       dying;
    static GhostState       ghosting;
	static FleeingState     fleeing;

    virtual ~PookaState() {}
    virtual void HandleInput(Pooka& ) {};
    virtual void Update(Pooka&, float ) {};

    virtual void OnEnter();
	virtual void OnExit();

protected:
    float m_AccTime{};
};

class MovingState : public PookaState
{
public:
	void HandleInput(Pooka&) override;
	void Update(Pooka& pooka, float deltaTime) override;

	void OnEnter() override;
	void OnExit() override;
};

class InflatedState : public PookaState
{
public:
	void HandleInput(Pooka&) override;
	void Update(Pooka& pooka, float deltaTime) override;

    void OnEnter() override;
    void OnExit() override;
};

class DeathState : public PookaState
{
public:
	void HandleInput(Pooka&) override;
	void Update(Pooka& pooka, float deltaTime) override;

    void OnEnter() override;
    void OnExit() override;
};

class GhostState : public PookaState
{
public:
    void HandleInput(Pooka&) override;
    void Update(Pooka& pooka, float deltaTime) override;

    void OnEnter() override;
    void OnExit() override;
};

class FleeingState : public PookaState
{
public:
    void HandleInput(Pooka&) override;
    void Update(Pooka& pooka, float deltaTime) override;

    void OnEnter() override;
    void OnExit() override;
};