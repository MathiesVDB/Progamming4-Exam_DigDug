#include "HighScore.h"
#include <fstream>
#include <sstream>
#include "Scene.h"
#include "SceneManager.h"

HighScore::HighScore()
	:	m_SaveScene			{ dae::SceneManager::GetInstance().CreateScene("HighScoreSaver") },
		m_HighScoresScene	{ dae::SceneManager::GetInstance().CreateScene("HighScoreShow") }
{
	
}

void HighScore::LoadSaveScene()
{
	
}

void HighScore::LoadHighScoreScene()
{
}


void HighScore::LoadHighScores()
{
    m_HighScores.clear();

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
