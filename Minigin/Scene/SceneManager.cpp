#include "SceneManager.h"
#include "Scene.h"
#include "SceneSwitcher.h"

void dae::SceneManager::Update(float deltaTime)
{
	GetActiveScene().Update(deltaTime);

	dae::SceneSwitcher::GetInstance().Update();
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
	auto it = std::find_if(m_scenes.begin(), m_scenes.end(),
		[&name](const std::shared_ptr<Scene>& scene)
		{
			return scene->GetName() == name;
		});

	//If scene already exists -> remove it
	if (it != m_scenes.end())
	{
		RemoveScene(name);
	}

	const auto& scene = std::shared_ptr<Scene>(new Scene(name, setActive));
	m_scenes.push_back(scene);
	return *scene;
}

void dae::SceneManager::RemoveScene(const std::string& name)
{
	auto iterator = std::find_if(m_scenes.begin(), m_scenes.end(),
		[&name](const std::shared_ptr<Scene>& scene)
		{
			return scene->GetName() == name;
		});

	if (iterator != m_scenes.end())
	{
		bool wasActive = (*iterator)->IsActive();

		m_scenes.erase(iterator);

		if (wasActive && !m_scenes.empty())
		{
			m_scenes.front()->SetActive(true);
		}
	}
	else
	{
		throw std::runtime_error("Scene with name '" + name + "' not found.");
	}
}