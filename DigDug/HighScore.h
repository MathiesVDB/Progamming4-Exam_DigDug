#pragma once
#include <string>
#include <utility>
#include <vector>

namespace dae
{
	class Scene;
}

class HighScore
{
public:
	HighScore();

	void LoadSaveScene();
	void LoadHighScoreScene();

private:
	void LoadHighScores();
	void SaveHighScores();

	dae::Scene& m_SaveScene;
	dae::Scene& m_HighScoresScene;

	std::vector<std::pair<std::string, int>> m_HighScores;
};

