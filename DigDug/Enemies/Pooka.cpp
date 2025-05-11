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

void Pooka::HandleCollision(const CollisionEvent& )
{
	std::cout << "Pooka handled collision!\n";
}

void Pooka::SetState(PookaState* state)
{
	m_State->OnExit(*this);
	m_State = state;
	m_State->OnEnter(*this);
}
