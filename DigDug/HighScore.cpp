#include "HighScore.h"
#include <fstream>
#include <sstream>
#include "Scene.h"
#include "SceneManager.h"
#include "ScoreHandler.h"

HighScore::HighScore(std::shared_ptr<ScoreHandler> scoreHandler)
	:	m_SaveScene			{ dae::SceneManager::GetInstance().CreateScene("HighScoreSaver") },
		m_HighScoresScene	{ dae::SceneManager::GetInstance().CreateScene("HighScoreShow") },
		m_ScoreHandler      { scoreHandler }
{
	
}

void HighScore::LoadSaveScene()
{
	
}

void HighScore::LoadHighScoreScene()
{
}


void HighScore::LoadHighScores(std::string chosenName)
{
    m_HighScores.clear();

    m_HighScores.emplace_back(std::pair<std::string, int>{chosenName, m_ScoreHandler->GetScore() });

    std::ifstream inFile("Data/HighScores.txt");
    if (!inFile)
    {
        std::cerr << "Failed to open HighScores.txt for reading\n";
        return;
    }

    std::string line;
    while (std::getline(inFile, line))
    {
        std::istringstream iss(line);
        std::string name;
        int score;

        if (!(iss >> name >> score))
        {
            std::cerr << "Invalid line format: " << line << '\n';
            continue;
        }

        m_HighScores.emplace_back(name, score);
    }

    std::sort(m_HighScores.begin(), m_HighScores.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

    //Trim to top 8
    if (m_HighScores.size() > 8)
        m_HighScores.resize(8);
}


void HighScore::SaveHighScores()
{
    std::ofstream outFile("Data/HighScores.txt", std::ios::trunc);
    if (!outFile)
    {
        std::cerr << "Failed to open HighScores.txt for writing\n";
        return;
    }

    for (const auto& [name, score] : m_HighScores)
    {
        outFile << name << ' ' << score << '\n';
    }
}
