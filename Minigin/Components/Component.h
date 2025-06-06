#pragma once

namespace dae
{
    class GameObject;

    class Component
    {
    public:
        Component(dae::GameObject* owner) : m_OwnerPtr{ owner } {}
        virtual ~Component() = default;

        Component& operator=(const Component& other) = delete;
        Component& operator=(Component&& other) = delete;

        virtual void Update(float deltaTime);
        virtual void Render() const;

        dae::GameObject* GetOwner() const;

    private:
        dae::GameObject* m_OwnerPtr{ nullptr };
    };
}