#include "ScoreDisplayer.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SelfDestructComponent.h"
#include "SpriteComponent.h"

ScoreDisplayer::ScoreDisplayer(dae::TextObject* mainScoreDisplay)
	:	m_MainScoreDisplay{mainScoreDisplay}
{

}

void ScoreDisplayer::ChangeScore(int score)
{
	std::string currentScore{ std::to_string(score) };

	m_MainScoreDisplay->SetText(currentScore);
}

void ScoreDisplayer::CreateNewScore(int score, const glm::vec2& pos)
{
	int frameIndex = -1;
	for (const auto& [scoreAmount, frame] : SCORE_FRAMES)
	{
		if (scoreAmount == score)
		{
			frameIndex = frame;
			break;
		}
	}

	if (frameIndex == -1)
	{
		std::cerr << "Unknown score: " << score << std::endl;
		return;
	}

	auto ScoreGameObject = std::make_unique<dae::GameObject>();
	ScoreGameObject->AddComponent<SpriteComponent>("Sprites/Misc/ScoreDarkSprite.png", 7, 2, 1.f, frameIndex, frameIndex);
	ScoreGameObject->AddComponent<SelfDestructComponent>(2.5f);
	ScoreGameObject->SetLocalPosition(pos);
	ScoreGameObject->SetRenderLayer(RenderLayer::Entity);

	m_DisplayedScores.emplace_back(ScoreGameObject.get());

	dae::SceneManager::GetInstance().GetActiveScene().Add(ScoreGameObject);
}