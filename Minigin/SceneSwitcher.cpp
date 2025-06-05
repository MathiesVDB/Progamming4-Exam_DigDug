#include "SceneSwitcher.h"
#include "SceneManager.h"

void dae::SceneSwitcher::QueueSceneChange(const std::string& sceneName)
{
	m_NextSceneFunc = [sceneName]()
		{
			auto& scene = SceneManager::GetInstance().GetScene(sceneName);
			SceneManager::GetInstance().SetActiveScene(scene);
		};
	m_HasPendingChange = true;
}

void dae::SceneSwitcher::QueueSceneChange(const std::function<void()>& loadSceneFunc)
{
	m_NextSceneFunc = loadSceneFunc;
	m_HasPendingChange = true;
}

void dae::SceneSwitcher::Update()
{
	if (m_HasPendingChange && m_NextSceneFunc)
	{
		m_NextSceneFunc();
		m_HasPendingChange = false;
	}
}