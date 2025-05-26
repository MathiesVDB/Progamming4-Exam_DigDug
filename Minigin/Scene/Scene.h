#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace dae
{
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name, bool setActive);
	public:
		void Add(std::unique_ptr<dae::GameObject>& object);
		void MarkForDeletion(dae::GameObject* object);
		void RemoveAll();

		void Update(float deltaTime);
		void Render() const;

		~Scene();
		Scene(const Scene& other)				= delete;
		Scene(Scene&& other)					= delete;
		Scene& operator=(const Scene& other)	= delete;
		Scene& operator=(Scene&& other)			= delete;

		const std::string& GetName() const { return m_name; }

		bool IsActive() const			{ return m_IsActive; }
		void SetActive(bool isActive)	{ m_IsActive = isActive; }

		dae::GameObject* GetPlayer(unsigned int index) const; // Get specific player by index
		dae::GameObject* GetEntity(const glm::vec2& position) const; // Get specific entity by position
		dae::GameObject* GetGround(const glm::vec2& position) const; // Get specific ground tile by position

	private: 
		explicit Scene(const std::string& name, bool setActive);

		void Remove(dae::GameObject* object); //Make sure markfordeletion is the only public option

		//Member variables
		std::string m_name;
		std::vector <std::unique_ptr<dae::GameObject>> m_Ground{};
		std::vector <std::unique_ptr<dae::GameObject>> m_Entities{};
		std::vector <std::unique_ptr<dae::GameObject>> m_Players{};
		std::vector<dae::GameObject*> m_PendingDeleteObjects;
		bool m_IsActive{ false };

		static unsigned int m_idCounter; 
	};

}
