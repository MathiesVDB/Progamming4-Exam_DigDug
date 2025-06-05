#include "SceneManager.h"
#include "Scene.h"
#include "SceneSwitcher.h"

void dae::SceneManager::Update(float deltaTime)
{
	GetActiveScene().Update(deltaTime);

	dae::SceneSwitcher::GetInstance().Update();
}

void dae::SceneManager::Render()
{
	GetActiveScene().Render();
}

dae::Scene& dae::SceneManager::GetActiveScene()
{
	if (m_scenes.empty()) return CreateScene("DefaultScene", true);
	for (const auto& scene : m_scenes)
	{
		if (scene->IsActive())
		{
			return *scene;
		}
	}
	// If no active scene, set first scene in vector to active
	m_scenes.front()->SetActive(true);
	return *m_scenes.front();
}

dae::Scene& dae::SceneManager::GetScene(const std::string& name) const
{
	for (const auto& scene : m_scenes)
	{
		if (scene->GetName() == name)
		{
			return *scene;
		}
	}
	throw std::runtime_error("Scene with name '" + name + "' not found.");
}

void dae::SceneManager::SetActiveScene(const Scene& newActiveScene) const
{
	for (const auto& scene : m_scenes)
	{
		if (scene->GetName() == newActiveScene.GetName())
		{
			scene->SetActive(true);
		}
		else
		{
			scene->SetActive(false);
		}
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name, bool setActive)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name, setActive));
	m_scenes.push_back(scene);
	return *scene;
}
