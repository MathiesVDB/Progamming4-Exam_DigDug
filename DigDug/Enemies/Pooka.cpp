#include "Pooka.h"

Pooka::Pooka(dae::GameObject* owner)
	:	Component(owner)
{
	m_State = std::make_unique<PookaState>();
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