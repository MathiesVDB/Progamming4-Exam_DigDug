#pragma once
#include <iostream>
#include <memory>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include "Component.h"
#include "Transform.h"

namespace dae
{
	class GameObject final
	{
	public:
		GameObject();
		virtual ~GameObject();

		GameObject(const GameObject& other)				= delete;
		GameObject(GameObject&& other)					= delete;
		GameObject& operator=(const GameObject& other)	= delete;
		GameObject& operator=(GameObject&& other)		= delete;

		void Update(float deltaTime);
		void Render() const;

		//Handle parent/child relationship
		void SetParent(GameObject* parent, bool keepWorldPosition);
		bool IsChild(GameObject* child) const;

        GameObject* GetParent() const { return m_Parent; }

		//Position functions
		const glm::vec3& GetLocalPosition() const;
        void SetLocalPosition(const glm::vec3& pos);

        const glm::vec3& GetWorldPosition();

        //Template functions
		template <typename T, typename... Args>
        std::shared_ptr<T> AddComponent(Args&&... args);

		template <typename T>
		void RemoveComponent();

		template <typename T>
        std::shared_ptr<T> GetComponent() const;

		template <typename T>
		bool HasComponent() const;



	private:
		//---------------------------------------------------------------------------------
        //Private Functions
        //---------------------------------------------------------------------------------
        void UpdateWorldPosition();
		void SetPositionDirty();

        void AddChild(GameObject* child);
        void RemoveChild(GameObject* child);

        //---------------------------------------------------------------------------------
		//Private member variables
        //---------------------------------------------------------------------------------
        std::unordered_map<std::type_index, std::shared_ptr<Component>> m_Components;
        glm::vec3 m_LocalPosition{ 0, 0, 0 };
        glm::vec3 m_WorldPosition{ 0, 0, 0 };
        bool m_IsPositionDirty{ true };

        GameObject* m_Parent{ nullptr };
        std::vector<GameObject*> m_Children;
	};

    // add component
    template <typename T, typename... Args>
    std::shared_ptr<T> GameObject::AddComponent(Args&&... args)
    {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from component");

        auto typeId = std::type_index(typeid(T));
        if (m_Components.find(typeId) == m_Components.end())
        {
            auto component = std::make_shared<T>(std::forward<Args>(args)...);
            component->SetOwner(this);
            m_Components[typeId] = component;
            return component;
        }

        return nullptr;
    }

    // remove component
    template <typename T>
    void GameObject::RemoveComponent()
    {
        auto typeId = std::type_index(typeid(T));
        m_Components.erase(typeId);
    }

    // get component
    template <typename T>
    std::shared_ptr<T> GameObject::GetComponent() const
    {
        auto typeId = std::type_index(typeid(T));
        auto it = m_Components.find(typeId);
        if (it != m_Components.end())
        {
            return std::dynamic_pointer_cast<T>(it->second);
        }

        return nullptr;
    }

    // has component 
    template <typename T>
    bool GameObject::HasComponent() const
    {
        return m_Components.find(std::type_index(typeid(T))) != m_Components.end();
    }
}
