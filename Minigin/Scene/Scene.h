#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace dae
{
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::unique_ptr<dae::GameObject>& object);
		void Remove(dae::GameObject* object);
		void RemoveAll();

		void Update(float deltaTime);
		void Render() const;

		~Scene();
		Scene(const Scene& other)				= delete;
		Scene(Scene&& other)					= delete;
		Scene& operator=(const Scene& other)	= delete;
		Scene& operator=(Scene&& other)			= delete;

	private: 
		explicit Scene(const std::string& name);

		//Member variables
		std::string m_name;
		std::vector <std::unique_ptr<dae::GameObject>> m_Ground{};
		std::vector <std::unique_ptr<dae::GameObject>> m_Entities{};
		std::vector <std::unique_ptr<dae::GameObject>> m_Players{};

		static unsigned int m_idCounter; 
	};

}
