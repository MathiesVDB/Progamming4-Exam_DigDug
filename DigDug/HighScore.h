#pragma once
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <array>

class ScoreHandler;

namespace dae
{
	class GameObject;
	class Scene;
}

class HighScore
{
public:
	HighScore(std::shared_ptr<ScoreHandler> scoreHandler);

	void LoadSaveScene();
	void LoadHighScoreScene();
	void FinishEntry();
	void UpdateLetterDisplay();

	//Getters
	std::array<char, 3> GetLetters() const { return m_NameLetters; }
	int GetLetterIndex() const { return m_SelectedLetterIndex; }
	std::array<dae::GameObject*, 3> GetLetterObjects() const { return m_LetterObjects; }

	//Setters
	void IncreaseLetterIndex() { ++m_SelectedLetterIndex; }
	void SetCurrentLetter(char newLetter) { m_NameLetters[m_SelectedLetterIndex] = newLetter; }

	static constexpr int FONT_SIZE{ 36 };

private:
	void LoadHighScores(std::string chosenName);
	void SaveHighScores();

	std::array<char, 3> m_NameLetters{ 'A', 'A', 'A' };
	int m_SelectedLetterIndex{ 0 };          
	bool m_EntryComplete{ false };

	dae::Scene& m_SaveScene;
	dae::Scene& m_HighScoresScene;

	std::vector<std::pair<std::string, int>> m_HighScores;
	std::array<dae::GameObject*, 3> m_LetterObjects;

	std::shared_ptr<ScoreHandler> m_ScoreHandler;

	dae::GameObject* m_UpArrowMarker	{ nullptr };
	dae::GameObject* m_DownArrowMarker	{ nullptr };
};

