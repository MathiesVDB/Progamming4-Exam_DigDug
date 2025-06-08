#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace dae
{
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name, bool setActive);
	public:
		void MarkForAdd(std::unique_ptr<dae::GameObject> object);
		void MarkForDeletion(dae::GameObject* object);
		void RemoveAll();

		void Update(float deltaTime);

		~Scene();
		Scene(const Scene& other)				= delete;
		Scene(Scene&& other)					= delete;
		Scene& operator=(const Scene& other)	= delete;
		Scene& operator=(Scene&& other)			= delete;

		const std::string& GetName() const { return m_name; }

		bool IsActive() const			{ return m_IsActive; }

		void SetActive(bool isActive)	{ m_IsActive = isActive; }

		dae::GameObject* GetObjectByPosition(const glm::vec2& position) const; // Get specific object by position

		const std::vector<std::unique_ptr<dae::GameObject>>& GetAllObjects() const { return m_SceneObjects;	}
		std::vector<dae::GameObject*> GetObjectsByTag(const std::string& tag) const;

	private: 
		explicit Scene(const std::string& name, bool setActive);

		void Remove(dae::GameObject* object); //Make sure markfordeletion is the only public option
		void Add(std::unique_ptr<dae::GameObject> object);

		//Member variables
		std::string m_name;
		std::vector <std::unique_ptr<dae::GameObject>> m_SceneObjects{};

		std::vector<std::unique_ptr<dae::GameObject>> m_PendingAddObjects;
		std::vector<dae::GameObject*> m_PendingDeleteObjects;

		bool m_IsActive{ false };

		static unsigned int m_idCounter; 
	};

}
