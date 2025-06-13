#include "GameDirector.h"
#include <utility>
#include "Level.h"
#include "FPSComponent.h"
#include "Fygar.h"
#include "InputManager.h"
#include "Pooka.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SceneSwitcher.h"

void GameDirector::Init(std::shared_ptr<SoundHandler> soundHandler, std::shared_ptr<ScoreHandler> scoreHandler)
{
	m_CurrentSceneName	= dae::SceneManager::GetInstance().GetActiveScene().GetName();
	m_SoundHandler		= std::move(soundHandler);
	m_ScoreHandler		= std::move(scoreHandler);

	dae::SceneSwitcher::GetInstance().QueueSceneChange([this]() {

		auto& scene = dae::SceneManager::GetInstance().CreateScene("StartScreen");

		auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

		auto logoGO = std::make_unique<dae::GameObject>();
		auto texture = logoGO->AddComponent<TextureComponent>("Sprites/Misc/StartLogo.png");
		logoGO->SetLocalPosition({dae::Minigin::WINDOW_WIDTH / 2.f - texture->GetWidth() / 2.f, 100.f});
		logoGO->SetObjectTag("UI");

		scene.MarkForAdd(std::move(logoGO));

		m_MenuOptions = { "1 Player", "2 Players", "Versus" };
		m_SelectedMenuIndex = 0;

		for (int index = 0; index < m_MenuOptions.size(); ++index)
		{
			auto menuGO = std::make_unique<dae::GameObject>();
			std::string label = (index == m_SelectedMenuIndex ? "> " : "  ") + m_MenuOptions[index];
			menuGO->AddComponent<dae::TextObject>(label, font);
			menuGO->SetLocalPosition({ dae::Minigin::WINDOW_WIDTH / 2.f - 100, 300.f + static_cast<float>(index) * 50.f });
			menuGO->SetObjectTag("UI");

			m_MenuButtons[index] = menuGO.get();
			scene.MarkForAdd(std::move(menuGO));
		}

		auto& inputManager = InputManager::GetInstance();

		// Keyboard commands
		inputManager.AddCommand(SDL_SCANCODE_W, KeyState::Down, std::make_unique<ButtonUpCommand>());
		inputManager.AddCommand(SDL_SCANCODE_S, KeyState::Down, std::make_unique<ButtonDownCommand>());

		inputManager.AddCommand(SDL_SCANCODE_C, KeyState::Down, std::make_unique<ButtonConfirmCommand>());

		// Controller commands
		inputManager.AddControllerCommand(0, SDL_CONTROLLER_BUTTON_DPAD_UP, KeyState::Down, std::make_unique<ButtonUpCommand>());
		inputManager.AddControllerCommand(0, SDL_CONTROLLER_BUTTON_DPAD_DOWN, KeyState::Down, std::make_unique<ButtonDownCommand>());

		inputManager.AddControllerCommand(0, SDL_CONTROLLER_BUTTON_A, KeyState::Down, std::make_unique<ButtonConfirmCommand>());

		dae::SceneManager::GetInstance().SetActiveScene(scene);
		});
}

void GameDirector::Notify(dae::GameObject* gameObject, dae::EventID event)
{
	if (!gameObject) return;

	std::string eventName = dae::EventRegistry::GetInstance().GetName(event);

	if (eventName == "EnemyDied")
	{
		int currentEnemies{ GetCurrentEnemies() };

		//Need to count one extra because counting happens before dead enemy gets removed from scene
		if		(currentEnemies == 2) FleeLastEnemy();
		else if (currentEnemies <= 1) SwitchToNextScene();
	}
	else if (eventName == "GameOver")
	{
		m_SceneFlow = SceneFlows::GameOver;
		SwitchToNextScene();
	}
}

void GameDirector::DetermineGameFlow()
{
	constexpr SceneFlows sceneFlows[] = {
		SceneFlows::SinglePlayer,
		SceneFlows::TwoPlayer,
		SceneFlows::Versus
	};

	if (m_SelectedMenuIndex >= 0 && m_SelectedMenuIndex < static_cast<int>(std::size(sceneFlows)))
	{
		m_SceneFlow = sceneFlows[m_SelectedMenuIndex];
	}

	SwitchToNextScene();
}

int GameDirector::GetCurrentEnemies()
{
	auto& activeScene{ dae::SceneManager::GetInstance().GetActiveScene() };

	if (activeScene.GetName() == m_CurrentSceneName) return --m_AliveEnemies;

	m_CurrentSceneName = activeScene.GetName();
	m_AliveEnemies = 0;
	const auto& objects{ activeScene.GetObjectsByTag("Enemy") };

	m_AliveEnemies = static_cast<int>(objects.size());

	return m_AliveEnemies;
}

void GameDirector::FleeLastEnemy()
{
	//Function only gets called on last enemy so first valid enemy is only enemy
	const auto& activeScene{ dae::SceneManager::GetInstance().GetActiveScene() };
	const auto& entities{ activeScene.GetObjectsByTag("Enemy") };

	for (const auto& entity : entities)
	{
		if (entity->HasComponent<Fygar>())
		{
			entity->GetComponent<Fygar>()->ToggleFleeing();
		}
		else if (entity->HasComponent<Pooka>())
		{
			entity->GetComponent<Pooka>()->ToggleFleeing();
		}
	}
}

void GameDirector::SwitchToNextScene()
{
	switch (m_SceneFlow)
	{
		case SceneFlows::SinglePlayer:
		{
			SingleplayerFlow();
			break;
		}
		case SceneFlows::TwoPlayer:
		{
			TwoplayerFlow();
			break;
		}
		case SceneFlows::Versus:
		{
			VersusFlow();
			break;
		}
		case SceneFlows::GameOver:
		{
			InputManager::GetInstance().ClearCommands();

			auto& scene = dae::SceneManager::GetInstance().GetActiveScene();

			const auto& objects = scene.GetAllObjects();
			for (const auto& object : objects)
			{
				if (object->GetObjectTag() != "Ground" && object->GetObjectTag() != "UI")
				{
					scene.MarkForDeletion(object.get());
				}
			}

			auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 48);

			auto gameOverText = std::make_unique<dae::GameObject>();
			gameOverText->AddComponent<dae::TextObject>("GAME OVER", font);
			gameOverText->SetLocalPosition({
				dae::Minigin::WINDOW_WIDTH / 2.f - 150.f, // Adjust as needed
				dae::Minigin::WINDOW_HEIGHT / 2.f - 24.f
				});
			gameOverText->SetObjectTag("UI");
			gameOverText->SetRenderLayer(3);

			scene.MarkForAdd(std::move(gameOverText));
			break;
		}
	}
}

void GameDirector::UpdateMenuVisuals()
{
	for (int counter = 0; counter < m_MenuOptions.size(); ++counter)
	{
		std::string label = (counter == m_SelectedMenuIndex ? "> " : "  ") + m_MenuOptions[counter];
		m_MenuButtons[counter]->GetComponent<dae::TextObject>()->SetText(label);
	}
}

void GameDirector::SingleplayerFlow()
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
				scene.MarkForAdd(std::move(FPSGameObject));

				auto levelGameObject = std::make_unique<dae::GameObject>();
				levelGameObject->AddComponent<Level>("Level1", soundHandler, scoreHandler, 3);
				levelGameObject->AddComponent<GridComponent>();
				levelGameObject->GetComponent<Level>()->LoadLevel("Level1.json");
				scene.MarkForAdd(std::move(levelGameObject));
				});

			m_Scene = Scenes::LEVEL1;
			break;
		}
		case Scenes::LEVEL1:
		{
			const auto& players{ dae::SceneManager::GetInstance().GetActiveScene().GetObjectsByTag("Player") };
			assert(!players.empty() && "NO PLAYERS FOUND");

			int player1Health{players[0]->GetComponent<Player>()->GetHealth()->GetLives()};

			auto soundHandler = m_SoundHandler;
			auto scoreHandler = m_ScoreHandler;

			dae::SceneSwitcher::GetInstance().QueueSceneChange([soundHandler, scoreHandler, player1Health]() {
				auto& scene = dae::SceneManager::GetInstance().CreateScene("MainScene");

				auto FPSGameObject = std::make_unique<dae::GameObject>();
				FPSGameObject->AddComponent<FPSComponent>();
				scene.MarkForAdd(std::move(FPSGameObject));

				auto levelGameObject = std::make_unique<dae::GameObject>();
				levelGameObject->AddComponent<Level>("Level2", soundHandler, scoreHandler, player1Health);
				levelGameObject->AddComponent<GridComponent>();
				levelGameObject->GetComponent<Level>()->LoadLevel("Level2.json");
				scene.MarkForAdd(std::move(levelGameObject));
				});

			m_Scene = Scenes::LEVEL2;
			break;
		}
		case Scenes::LEVEL2:
		{
			const auto& players{ dae::SceneManager::GetInstance().GetActiveScene().GetObjectsByTag("Player") };
			assert(!players.empty() && "NO PLAYERS FOUND");

			int player1Health{ players[0]->GetComponent<Player>()->GetHealth()->GetLives() };

			auto soundHandler = m_SoundHandler;
			auto scoreHandler = m_ScoreHandler;

			dae::SceneSwitcher::GetInstance().QueueSceneChange([soundHandler, scoreHandler, player1Health]() {
				auto& scene = dae::SceneManager::GetInstance().CreateScene("MainScene");

				auto FPSGameObject = std::make_unique<dae::GameObject>();
				FPSGameObject->AddComponent<FPSComponent>();
				scene.MarkForAdd(std::move(FPSGameObject));

				auto levelGameObject = std::make_unique<dae::GameObject>();
				levelGameObject->AddComponent<Level>("Level3", soundHandler, scoreHandler, player1Health);
				levelGameObject->AddComponent<GridComponent>();
				levelGameObject->GetComponent<Level>()->LoadLevel("Level3.json");
				scene.MarkForAdd(std::move(levelGameObject));
				});

			m_Scene = Scenes::LEVEL3;
			break;
		}
		case Scenes::LEVEL3:
		{
			dae::SceneSwitcher::GetInstance().QueueSceneChange([this]() {

				m_HighScore = std::make_unique<HighScore>(m_ScoreHandler);
				m_HighScore->LoadSaveScene();
				});

			m_Scene = Scenes::VICTORY;
			break;
		}
	}
}

void GameDirector::TwoplayerFlow()
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
			scene.MarkForAdd(std::move(FPSGameObject));

			auto levelGameObject = std::make_unique<dae::GameObject>();
			levelGameObject->AddComponent<Level>("CoopLevel1", soundHandler, scoreHandler, 3);
			levelGameObject->AddComponent<GridComponent>();
			levelGameObject->GetComponent<Level>()->LoadLevel("CoopLevel1.json");
			scene.MarkForAdd(std::move(levelGameObject));
			});

		m_Scene = Scenes::LEVEL1;
		break;
	}
	case Scenes::LEVEL1:
	{
		const auto& players{ dae::SceneManager::GetInstance().GetActiveScene().GetObjectsByTag("Player") };
		assert(!players.empty() && "NO PLAYERS FOUND");

		int player1Health{ players[0]->GetComponent<Player>()->GetHealth()->GetLives() };

		auto soundHandler = m_SoundHandler;
		auto scoreHandler = m_ScoreHandler;

		dae::SceneSwitcher::GetInstance().QueueSceneChange([soundHandler, scoreHandler, player1Health]() {
			auto& scene = dae::SceneManager::GetInstance().CreateScene("MainScene");

			auto FPSGameObject = std::make_unique<dae::GameObject>();
			FPSGameObject->AddComponent<FPSComponent>();
			scene.MarkForAdd(std::move(FPSGameObject));

			auto levelGameObject = std::make_unique<dae::GameObject>();
			levelGameObject->AddComponent<Level>("CoopLevel2", soundHandler, scoreHandler, player1Health);
			levelGameObject->AddComponent<GridComponent>();
			levelGameObject->GetComponent<Level>()->LoadLevel("CoopLevel2.json");
			scene.MarkForAdd(std::move(levelGameObject));
			});

		m_Scene = Scenes::LEVEL2;
		break;
	}
	case Scenes::LEVEL2:
	{
		const auto& players{ dae::SceneManager::GetInstance().GetActiveScene().GetObjectsByTag("Player") };
		assert(!players.empty() && "NO PLAYERS FOUND");

		int player1Health{ players[0]->GetComponent<Player>()->GetHealth()->GetLives() };

		auto soundHandler = m_SoundHandler;
		auto scoreHandler = m_ScoreHandler;

		dae::SceneSwitcher::GetInstance().QueueSceneChange([soundHandler, scoreHandler, player1Health]() {
			auto& scene = dae::SceneManager::GetInstance().CreateScene("MainScene");

			auto FPSGameObject = std::make_unique<dae::GameObject>();
			FPSGameObject->AddComponent<FPSComponent>();
			scene.MarkForAdd(std::move(FPSGameObject));

			auto levelGameObject = std::make_unique<dae::GameObject>();
			levelGameObject->AddComponent<Level>("CoopLevel3", soundHandler, scoreHandler, player1Health);
			levelGameObject->AddComponent<GridComponent>();
			levelGameObject->GetComponent<Level>()->LoadLevel("CoopLevel3.json");
			scene.MarkForAdd(std::move(levelGameObject));
			});

		m_Scene = Scenes::LEVEL3;
		break;
	}
	case Scenes::LEVEL3:
	{
		dae::SceneSwitcher::GetInstance().QueueSceneChange([]() {
			auto& scene = dae::SceneManager::GetInstance().CreateScene("CoopVictoryScene", true);

			auto roundClearGO = std::make_unique<dae::GameObject>();
			auto texture = roundClearGO->AddComponent<TextureComponent>("Sprites/Misc/RoundClear.png");
			roundClearGO->SetLocalPosition({
				dae::Minigin::WINDOW_WIDTH / 2.f - texture->GetWidth() / 2.f,
				100.f
				});
			roundClearGO->SetObjectTag("UI");
			scene.MarkForAdd(std::move(roundClearGO));
			});

		m_Scene = Scenes::VICTORY;
		break;
	}
	}
}

void GameDirector::VersusFlow()
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
			scene.MarkForAdd(std::move(FPSGameObject));

			auto levelGameObject = std::make_unique<dae::GameObject>();
			levelGameObject->AddComponent<Level>("VersusLevel1", soundHandler, scoreHandler, 3);
			levelGameObject->AddComponent<GridComponent>();
			levelGameObject->GetComponent<Level>()->LoadLevel("VersusLevel1.json");
			scene.MarkForAdd(std::move(levelGameObject));
			});

		m_Scene = Scenes::LEVEL1;
		break;
	}
	case Scenes::LEVEL1:
	{
		const auto& players{ dae::SceneManager::GetInstance().GetActiveScene().GetObjectsByTag("Player") };
		assert(!players.empty() && "NO PLAYERS FOUND");

		int player1Health{ players[0]->GetComponent<Player>()->GetHealth()->GetLives() };

		auto soundHandler = m_SoundHandler;
		auto scoreHandler = m_ScoreHandler;

		dae::SceneSwitcher::GetInstance().QueueSceneChange([soundHandler, scoreHandler, player1Health]() {
			auto& scene = dae::SceneManager::GetInstance().CreateScene("MainScene");

			auto FPSGameObject = std::make_unique<dae::GameObject>();
			FPSGameObject->AddComponent<FPSComponent>();
			scene.MarkForAdd(std::move(FPSGameObject));

			auto levelGameObject = std::make_unique<dae::GameObject>();
			levelGameObject->AddComponent<Level>("VersusLevel2", soundHandler, scoreHandler, player1Health);
			levelGameObject->AddComponent<GridComponent>();
			levelGameObject->GetComponent<Level>()->LoadLevel("VersusLevel2.json");
			scene.MarkForAdd(std::move(levelGameObject));
			});

		m_Scene = Scenes::LEVEL2;
		break;
	}
	case Scenes::LEVEL2:
	{
		const auto& players{ dae::SceneManager::GetInstance().GetActiveScene().GetObjectsByTag("Player") };
		assert(!players.empty() && "NO PLAYERS FOUND");

		int player1Health{ players[0]->GetComponent<Player>()->GetHealth()->GetLives() };

		auto soundHandler = m_SoundHandler;
		auto scoreHandler = m_ScoreHandler;

		dae::SceneSwitcher::GetInstance().QueueSceneChange([soundHandler, scoreHandler, player1Health]() {
			auto& scene = dae::SceneManager::GetInstance().CreateScene("MainScene");

			auto FPSGameObject = std::make_unique<dae::GameObject>();
			FPSGameObject->AddComponent<FPSComponent>();
			scene.MarkForAdd(std::move(FPSGameObject));

			auto levelGameObject = std::make_unique<dae::GameObject>();
			levelGameObject->AddComponent<Level>("VersusLevel3", soundHandler, scoreHandler, player1Health);
			levelGameObject->AddComponent<GridComponent>();
			levelGameObject->GetComponent<Level>()->LoadLevel("VersusLevel3.json");
			scene.MarkForAdd(std::move(levelGameObject));
			});

		m_Scene = Scenes::LEVEL3;
		break;
	}
	case Scenes::LEVEL3:
	{
		dae::SceneSwitcher::GetInstance().QueueSceneChange([]() {
			auto& scene = dae::SceneManager::GetInstance().CreateScene("VersusVictoryScene", true);

			auto roundClearGO = std::make_unique<dae::GameObject>();
			auto texture = roundClearGO->AddComponent<TextureComponent>("Sprites/Misc/RoundClear.png");
			roundClearGO->SetLocalPosition({
				dae::Minigin::WINDOW_WIDTH / 2.f - texture->GetWidth() / 2.f,
				100.f
				});
			roundClearGO->SetObjectTag("UI");
			scene.MarkForAdd(std::move(roundClearGO));
			});

		m_Scene = Scenes::VICTORY;
		break;
	}
	}
}