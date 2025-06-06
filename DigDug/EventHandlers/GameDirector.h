#pragma once
#include "Observer.h"
#include "HighScore.h"

class SoundHandler;
class ScoreHandler;

namespace dae
{
	class Scene;
}

class GameDirector : public dae::Singleton<GameDirector>
{
public:
	void Init(std::shared_ptr<SoundHandler> soundHandler, std::shared_ptr<ScoreHandler> scoreHandler);

	void Notify(dae::GameObject* gameObject, dae::EventID event);

	//Main menu function
	void UpdateMenuVisuals();

	int GetSelectedMenuIndex() const { return m_SelectedMenuIndex; }
	std::vector<std::string> GetMenuOptions() const { return m_MenuOptions; }

	void IncreaseMenuIndex() { ++m_SelectedMenuIndex; }
	void DecreaseMenuIndex() { --m_SelectedMenuIndex; }

	void DetermineGameFlow();

private:
	friend class dae::Singleton<GameDirector>;

	GameDirector() = default;

	//Private enum of all scenes
	enum class SceneFlows
	{
		SinglePlayer,
		TwoPlayer,
		Versus
	};

	enum class Scenes
	{
		MAIN,
		LEVEL1,
		HIGHSCORE
	};

	//Private functions
	int GetCurrentEnemies();
	void FleeLastEnemy();
	void SwitchToNextScene();

	void SingleplayerFlow();
	void TwoplayerFlow();
	void VersusFlow();

	//Member variables
	SceneFlows m_SceneFlow{ SceneFlows::SinglePlayer };
	Scenes m_Scene{ Scenes::MAIN };

	std::string m_CurrentSceneName;

	int m_AliveEnemies{};

	std::unique_ptr<HighScore>		m_HighScore;
	std::shared_ptr<SoundHandler>	m_SoundHandler;
	std::shared_ptr<ScoreHandler>	m_ScoreHandler;

	//Main menu variables
	std::unordered_map<int, dae::GameObject*> m_MenuButtons{};
	std::vector<std::string> m_MenuOptions{};
	int m_SelectedMenuIndex{};
};

