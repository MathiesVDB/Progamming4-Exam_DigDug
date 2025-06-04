#pragma once
#include "TextObject.h"
#include <array>

class ScoreDisplayer
{
public:
	ScoreDisplayer(dae::TextObject* mainScoreDisplay);

	void AddScore(int score);

	void CreateNewScore(int score, const glm::vec2& pos);
	void ChangeMainScore();

	//Score sprites
    static constexpr std::array<std::pair<int, int>, 14> SCORE_FRAMES = { {
        {200,    0},
        {300,    2},
        {400,    4},
        {500,    6},
        {600,    8},
        {800,   10},
        {1000,  12},
        {2500,   1},
        {4000,   3},
        {6000,   5},
        {8000,   7},
        {10000,  9},
        {12000, 11},
        {15000, 13}
    } };

private:
	int m_Score{};

	dae::TextObject* m_MainScoreDisplay;

	std::vector<dae::GameObject*> m_DisplayedScores;
};
