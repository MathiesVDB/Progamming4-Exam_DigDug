#include "ScoreHandler.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Fygar.h"
#include "ScoreDisplayer.h"

ScoreHandler::ScoreHandler(ScoreDisplayer* display)
	:   m_Displayer{display}
{
}

void ScoreHandler::Notify(dae::GameObject* gameObject, dae::EventID event)
{
	if (!gameObject) return;

	std::string eventName = dae::EventRegistry::GetInstance().GetName(event);

	if (eventName == "EnemyDied")
	{
        const auto position = gameObject->GetWorldPosition();
        int scoreToAdd = 0;

        // Determine base score by layer
        if (position.y < SECOND_LAYER_Y)
        {
            scoreToAdd = FIRST_LAYER_SCORE;
        }
        else if (position.y < THIRD_LAYER_Y)
        {
            scoreToAdd = SECOND_LAYER_SCORE;
        }
        else if (position.y < FOURTH_LAYER_Y)
        {
            scoreToAdd = THIRD_LAYER_SCORE;
        }
        else
        {
            scoreToAdd = FOURTH_LAYER_SCORE;
        }

        if (gameObject->HasComponent<Fygar>())
        {
            const float MARGIN{ 2.0f };

            auto player = dae::SceneManager::GetInstance().GetActiveScene().GetPlayer(0);
            float playerY = player->GetWorldPosition().y;

            if (std::abs(position.y - playerY) <= MARGIN)
            {
                scoreToAdd *= 2;
            }
        }

        m_Displayer->AddScore(scoreToAdd);
        m_Displayer->CreateNewScore(scoreToAdd, gameObject->GetWorldPosition());
	}
}