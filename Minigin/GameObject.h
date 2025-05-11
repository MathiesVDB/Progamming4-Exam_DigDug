#pragma once
#include <iostream>
#include <memory>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include "Component.h"
#include "Helpers.h"
#include "Transform.h"
#include "TextObject.h"

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

        //Render layer
        void SetRenderLayer(RenderLayer layer) { m_RenderLayer = layer; }
        RenderLayer GetRenderLayer() const { return m_RenderLayer; }

		//Handle parent/child relationship
		void SetParent(GameObject* parent, bool keepWorldPosition);
		bool IsChild(GameObject* child) const;

        GameObject* GetParent() const { return m_Parent; }

		//Position functions
		const glm::vec3& GetLocalPosition() const;
        void SetLocalPosition(const glm::vec3& pos);

        const glm::vec3& GetWorldPosition();

		//Velocity functions
		const glm::vec3& GetVelocity() const;
		void SetVelocity(const glm::vec3& velocity);
		void AddVelocity(const glm::vec3& velocity);

        //Template functions
		template <typename T, typename... Args>
        T* AddComponent(Args&&... args);

		template <typename T>
		void RemoveComponent();

		template <typename T>
        T* GetComponent() const;

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
        std::unordered_map<std::type_index, std::unique_ptr<Component>> m_Components;
        glm::vec3 m_LocalPosition   { 0, 0, 0 };
        glm::vec3 m_WorldPosition   { 0, 0, 0 };
		glm::vec3 m_Velocity        { 0, 0, 0 };
        RenderLayer m_RenderLayer{};
        bool m_IsPositionDirty{ true };

        GameObject* m_Parent{ nullptr };
        std::vector<GameObject*> m_Children;
	};

    // add component
    template <typename T, typename... Args>
    T* GameObject::AddComponent(Args&&... args)
    {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from component");

        auto typeId = std::type_index(typeid(T));
        if (m_Components.find(typeId) == m_Components.end())
        {
            auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
            T* pointer = component.get();
            m_Components[typeId] = std::move(component);
            return pointer;
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
    T* GameObject::GetComponent() const
    {
        auto typeId = std::type_index(typeid(T));
        auto it = m_Components.find(typeId);
        if (it != m_Components.end())
        {
            return static_cast<T*>(it->second.get());
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
