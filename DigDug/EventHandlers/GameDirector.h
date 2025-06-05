#pragma once
#include "Observer.h"

class SoundHandler;
class ScoreHandler;

namespace dae
{
	class Scene;
}

class GameDirector : public dae::Observer
{
public:
	GameDirector(std::shared_ptr<SoundHandler> soundHandler, std::shared_ptr<ScoreHandler> scoreHandler);

	void Notify(dae::GameObject* gameObject, dae::EventID event) override;

private:
	//Private enum of all scenes
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

	//Member variables
	Scenes m_Scene{ Scenes::MAIN };

	std::string m_CurrentSceneName;

	int m_AliveEnemies{};

	std::shared_ptr<SoundHandler>	m_SoundHandler;
	std::shared_ptr<ScoreHandler>	m_ScoreHandler;
};

