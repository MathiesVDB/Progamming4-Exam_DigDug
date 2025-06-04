#pragma once
#include "Observer.h"
#include <array>

class Player;
class ScoreDisplayer;

class ScoreHandler : public dae::Observer
{
public:
	ScoreHandler(ScoreDisplayer* display);

	void Notify(dae::GameObject* gameObject, dae::EventID event) override;

	//Constants
	//Layers
	static constexpr int SECOND_LAYER_Y	{ 240 };
	static constexpr int THIRD_LAYER_Y	{ 400 };
	static constexpr int FOURTH_LAYER_Y	{ 560 };

	//Scores
	static constexpr int FIRST_LAYER_SCORE { 200 };
	static constexpr int SECOND_LAYER_SCORE{ 300 };
	static constexpr int THIRD_LAYER_SCORE { 400 };
	static constexpr int FOURTH_LAYER_SCORE{ 500 };

	static constexpr std::array<int, 8> ROCK_SCORES{
		1000,
		2500,
		4000,
		6000,
		8000,
		10000,
		12000,
		15000
	};

private:
	ScoreDisplayer* m_Displayer;
};