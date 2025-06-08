#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name, bool setActive = false);

		void Update(float deltaTime);

		Scene& GetActiveScene();
		Scene& GetScene(const std::string& name) const;
		void SetActiveScene(const Scene& newActiveScene) const;

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;
	};
}
