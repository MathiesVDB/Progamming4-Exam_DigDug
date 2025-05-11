#pragma once

class Pooka;

namespace PookaStates
{
    class MovingState;
    class InflatedState;
    class DeathState;
    class GhostState;
    class FleeingState;
    
    class PookaState 
    {
    public:
        static MovingState      moving;
        static InflatedState    inflating;
        static DeathState       dying;
        static GhostState       ghosting;
    	static FleeingState     fleeing;
    
        virtual ~PookaState() {}
        virtual void HandleInput(Pooka& ) {}
        virtual void Update(Pooka& , float ) {}
    
        virtual void OnEnter(Pooka& ) {}
    	virtual void OnExit(Pooka& ) {}
    
    protected:
        float m_AccTime{};
    };
    
    class MovingState : public PookaState
    {
    public:
    	void HandleInput(Pooka& pooka) override;
    	void Update(Pooka& pooka, float deltaTime) override;
    
    	void OnEnter(Pooka& pooka) override;
    	void OnExit(Pooka& pooka) override;
    };
    
    class InflatedState : public PookaState
    {
    public:
    	void HandleInput(Pooka& pooka) override;
    	void Update(Pooka& pooka, float deltaTime) override;
    
        void OnEnter(Pooka& pooka) override;
        void OnExit(Pooka& pooka) override;
    };
    
    class DeathState : public PookaState
    {
    public:
    	void HandleInput(Pooka& pooka) override;
    	void Update(Pooka& pooka, float deltaTime) override;
    
        void OnEnter(Pooka& pooka) override;
        void OnExit(Pooka& pooka) override;
    };
    
    class GhostState : public PookaState
    {
    public:
        void HandleInput(Pooka& pooka) override;
        void Update(Pooka& pooka, float deltaTime) override;
    
        void OnEnter(Pooka& pooka) override;
        void OnExit(Pooka& pooka) override;
    };
    
    class FleeingState : public PookaState
    {
    public:
        void HandleInput(Pooka& pooka) override;
        void Update(Pooka& pooka, float deltaTime) override;
    
        void OnEnter(Pooka& pooka) override;
        void OnExit(Pooka& pooka) override;
    };
    
}