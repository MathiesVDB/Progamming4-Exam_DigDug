#include "Transform.h"
#include "GameObject.h"

namespace dae
{
    void Transform::Update(float)
    {
        
    }

	void Transform::SetPosition(const float x, const float y)
	{
		GetOwner()->SetLocalPosition({ x, y });
	}

	const glm::vec2& Transform::GetPosition() const
	{
		return GetOwner() ? GetOwner()->GetWorldPosition() : m_Position;
	}
}