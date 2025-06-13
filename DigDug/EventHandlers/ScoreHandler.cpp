#include "ScoreHandler.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Fygar.h"
#include "GameDirector.h"
#include "Pooka.h"
#include "Rock.h"
#include "ScoreDisplayer.h"

ScoreHandler::ScoreHandler()
{
}

void ScoreHandler::Notify(dae::GameObject* gameObject, dae::EventID event)
{
	if (!gameObject) return;

	std::string eventName = dae::EventRegistry::GetInstance().GetName(event);

    GameDirector::GetInstance().Notify(gameObject, event);

	if (eventName == "EnemyDied")
	{
        //Rock scores will be handled separately so if crushed no need for score calculation
        if (gameObject->HasComponent<Fygar>())
        {
            if (gameObject->GetComponent<Fygar>()->WasCrushed()) return;
        }
        else if (gameObject->HasComponent<Pooka>())
        {
            if (gameObject->GetComponent<Pooka>()->WasCrushed()) return;
        }

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

            const auto& players{ dae::SceneManager::GetInstance().GetActiveScene().GetObjectsByTag("Player") };

            assert(!players.empty() && "NO PLAYERS FOUND");

            auto player = players[0];
            float playerY = player->GetWorldPosition().y;

            if (std::abs(position.y - playerY) <= MARGIN)
            {
                scoreToAdd *= 2;
            }
        }

        m_Displayer->CreateNewScore(scoreToAdd, gameObject->GetWorldPosition());

        m_Score += scoreToAdd;

        m_Displayer->ChangeScore(m_Score);
	}
    else if (eventName == "RockBroke")
    {
        if (!gameObject->HasComponent<Rock>()) return;

        int crushCount{ gameObject->GetComponent<Rock>()->GetCrushCount() };

        int scoreToAdd = 0;
        switch (crushCount)
        {
        case 1: scoreToAdd = 1000; break;
        case 2: scoreToAdd = 2500; break;
        case 3: scoreToAdd = 4000; break;
        case 4: scoreToAdd = 6000; break;
        case 5: scoreToAdd = 8000; break;
        case 6: scoreToAdd = 10000; break;
        case 7: scoreToAdd = 12000; break;
        case 8: scoreToAdd = 15000; break;
        default: return;
        }

        m_Displayer->CreateNewScore(scoreToAdd, gameObject->GetWorldPosition());

        m_Score += scoreToAdd;

        m_Displayer->ChangeScore(m_Score);
    }
}