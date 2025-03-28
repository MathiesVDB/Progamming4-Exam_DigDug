#include "GameObject.h"

#include <windows.h>

namespace dae
{
	GameObject::GameObject()
	{
        AddComponent<Transform>();
	}

    GameObject::~GameObject() = default;

	void GameObject::Update(float deltaTime)
    {
        for (const auto& [type, component] : m_Components)
        {
            component->Update(deltaTime);
        }
    }

    void GameObject::Render() const
    {
        for (const auto& [type, component] : m_Components)
        {
            component->Render();
        }
    }

    void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
    {
        if (IsChild(parent) || parent == this || m_Parent == parent) return;

        if (parent == nullptr)
        {
            SetLocalPosition(GetWorldPosition());
        }
        else
        {
	        if (keepWorldPosition)
	        {
				SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
	        }

            SetPositionDirty();
        }

		if (m_Parent) m_Parent->RemoveChild(this);
        m_Parent = parent;
		if (m_Parent) m_Parent->AddChild(this);
    }

    void GameObject::AddChild(GameObject* child)
    {
        if (child && !IsChild(child))
        {
            m_Children.push_back(child);
        }
    }

    void GameObject::RemoveChild(GameObject* child)
    {
        m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child), m_Children.end());
    }

    bool GameObject::IsChild(GameObject* potentialChild) const
    {
        return std::find(m_Children.begin(), m_Children.end(), potentialChild) != m_Children.end();
    }

    const glm::vec3& GameObject::GetLocalPosition() const
	{
		return m_LocalPosition;
    }

    void GameObject::SetLocalPosition(const glm::vec3& pos)
    {
		m_LocalPosition = pos;
		SetPositionDirty();
    }

    const glm::vec3& GameObject::GetWorldPosition()
    {
		if (m_IsPositionDirty)
		{
			UpdateWorldPosition();
		}
		return m_WorldPosition;
    }

    void GameObject::UpdateWorldPosition()
    {
        if (m_IsPositionDirty)
        {
			if (m_Parent == nullptr)
			{
				m_WorldPosition = m_LocalPosition;
			}
			else
			{
				m_WorldPosition = m_Parent->GetWorldPosition() + m_LocalPosition;
			}
        }

		m_IsPositionDirty = false;
    }

    void GameObject::SetPositionDirty()
    {
        m_IsPositionDirty = true;
        for (auto* child : m_Children)
        {
            child->SetPositionDirty();
        }
    }
}
