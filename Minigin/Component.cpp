//---------------------------
// Include Files
//---------------------------
#include "Component.h"

#include <iostream>

namespace dae
{
	class GameObject;
}

//---------------------------
// Member functions
//---------------------------
void Component::Update(float)
{
}

void Component::Render() const
{
}

void Component::SetOwner(dae::GameObject* owner)
{
    if (m_OwnerPtr != nullptr)
    {
        std::cerr << "Warning: Component already has an owner.\n";
    }
    m_OwnerPtr = owner;
}

dae::GameObject* Component::GetOwner() const
{
    if (m_OwnerPtr != nullptr) return m_OwnerPtr;

    return nullptr;
}