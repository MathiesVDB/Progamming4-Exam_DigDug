//---------------------------
// Include Files
//---------------------------
#include "Component.h"

#include <iostream>

namespace dae
{
    class GameObject;

    //---------------------------
    // Member functions
    //---------------------------
    void Component::Update(float)
    {
    }

    void Component::Render() const
    {
    }

    dae::GameObject* Component::GetOwner() const
    {
        if (m_OwnerPtr != nullptr) return m_OwnerPtr;

        return nullptr;
    }
}