#include "Transform.h"
#include "GameObject.h"

namespace dae
{
    void Transform::Update(float)
    {
    }


    void Transform::SetPosition(const float x, const float y, const float z)
    {
        GetOwner()->SetLocalPosition({ x, y, z });
    }

    const glm::vec3& Transform::GetPosition() const
    {
        return GetOwner() ? GetOwner()->GetWorldPosition() : m_Position;
    }
}