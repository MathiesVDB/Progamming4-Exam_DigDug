#pragma once

namespace dae
{
	class GameObject;
}

class Component
{
public:
    virtual ~Component() = default;

    Component& operator=(const Component& other)    = delete;
    Component& operator=(Component&& other)         = delete;

    virtual void Update(float deltaTime);
    virtual void Render() const;

    void SetOwner(dae::GameObject* owner);
	dae::GameObject* GetOwner() const;

private:
    dae::GameObject* m_OwnerPtr{ nullptr };
};