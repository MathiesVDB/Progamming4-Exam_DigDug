#pragma once
#include <memory>
#include <string>
#include <utility>
#include <vector>

class ScoreHandler;

namespace dae
{
	class Scene;
}

class HighScore
{
public:
	HighScore(std::shared_ptr<ScoreHandler> scoreHandler);

	void LoadSaveScene();
	void LoadHighScoreScene();

private:
	void LoadHighScores(std::string chosenName);
	void SaveHighScores();

	dae::Scene& m_SaveScene;
	dae::Scene& m_HighScoresScene;

	std::vector<std::pair<std::string, int>> m_HighScores;

	std::shared_ptr<ScoreHandler> m_ScoreHandler;
};

