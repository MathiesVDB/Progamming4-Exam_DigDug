#include "GameDirector.h"
#include <utility>
#include "Level.h"
#include "FPSComponent.h"
#include "Fygar.h"
#include "Pooka.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SceneSwitcher.h"
#include "HighScore.h"

GameDirector::GameDirector(std::shared_ptr<SoundHandler> soundHandler, std::shared_ptr<ScoreHandler> scoreHandler)
	:	m_CurrentSceneName{ dae::SceneManager::GetInstance().GetActiveScene().GetName() },
		m_SoundHandler{ std::move(soundHandler)},
		m_ScoreHandler{ std::move(scoreHandler)}
{
}

void GameDirector::Notify(dae::GameObject* gameObject, dae::EventID event)
{
	if (!gameObject) return;

	std::string eventName = dae::EventRegistry::GetInstance().GetName(event);

	if (eventName == "EnemyDied")
	{
		if (GetCurrentEnemies() == 1) FleeLastEnemy();
		else if (GetCurrentEnemies() <= 0) SwitchToNextScene();
	}
}

int GameDirector::GetCurrentEnemies()
{
	auto& activeScene{ dae::SceneManager::GetInstance().GetActiveScene() };

	if (activeScene.GetName() == m_CurrentSceneName) return --m_AliveEnemies;

	m_CurrentSceneName = activeScene.GetName();
	m_AliveEnemies = 0;
	auto entities{ activeScene.GetAllEntities() };

	for (const auto& entity : entities)
	{
		if (entity->HasComponent<Fygar>() || entity->HasComponent<Pooka>()) ++m_AliveEnemies;
	}

	return m_AliveEnemies;
}

void GameDirector::FleeLastEnemy()
{
	//Function only gets called on last enemy so first valid enemy is only enemy
	auto& activeScene{ dae::SceneManager::GetInstance().GetActiveScene() };
	auto entities{ activeScene.GetAllEntities() };

	for (const auto& entity : entities)
	{
		if (entity->HasComponent<Fygar>())
		{
			entity->GetComponent<Fygar>()->ToggleFleeing();
			return;
		}
		if (entity->HasComponent<Pooka>())
		{
			entity->GetComponent<Pooka>()->ToggleFleeing();
			return;
		}
	}
}

void GameDirector::SwitchToNextScene()
{
	switch (m_Scene)
	{
	case Scenes::MAIN:
		{
		auto soundHandler = m_SoundHandler;
		auto scoreHandler = m_ScoreHandler;

		dae::SceneSwitcher::GetInstance().QueueSceneChange([soundHandler, scoreHandler]() {
			auto& scene = dae::SceneManager::GetInstance().CreateScene("MainScene");

			auto FPSGameObject = std::make_unique<dae::GameObject>();
			FPSGameObject->AddComponent<FPSComponent>();
			scene.Add(FPSGameObject);

			auto levelGameObject = std::make_unique<dae::GameObject>();
			levelGameObject->AddComponent<Level>("Level1", soundHandler, scoreHandler);
			levelGameObject->AddComponent<GridComponent>();
			levelGameObject->GetComponent<Level>()->LoadLevel("Level1.json");
			scene.Add(levelGameObject);
			});
		break;
		}
	case Scenes::LEVEL1:
		{
		std::unique_ptr<HighScore> highScore{ std::make_unique<HighScore>(m_ScoreHandler) };
		highScore->LoadSaveScene();
		}
	}
}
