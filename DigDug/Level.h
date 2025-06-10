#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GridComponent.h"
#include "Component.h"
#include "Scene.h"
#include "ScoreDisplayer.h"
#include "SoundHandler.h"

class GameDirector;
class ScoreHandler;
class RopeComponent;
//-----------------------------------------------------
// Level Class									 
//-----------------------------------------------------

enum class Inflated
{
	None,
	Stage1,
	Stage2,
	Stage3,
	Exploded
};

class Level final : public dae::Component
{
public:
	Level(dae::GameObject* owner, const std::string& sceneName, std::shared_ptr<SoundHandler> soundHandler, std::shared_ptr<ScoreHandler> scoreHandler, int player1Health, int player2Health = 0);

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	void LoadLevel(const std::string& fileName); // Will look for file in ../Data/Levels/

	//Public because used in player
	std::unique_ptr<dae::GameObject> SpawnEmpty(const glm::vec2& spawnPos) const;
	std::unique_ptr<dae::GameObject> SpawnRope(const glm::vec2& spawnPos) const;

	std::string GetSceneName() const { return m_Scene.GetName(); }

private:
	//-------------------------------------------------
	// Private member functions								
	//-------------------------------------------------
	void SpawnPlayer			(const glm::vec2& spawnPos);
	void SpawnPooka				(const glm::vec2& spawnPos)	const;
	void SpawnFygar				(const glm::vec2& spawnPos, bool isControlled = false)	const;
	void SpawnRock				(const glm::vec2& spawnPos)	const;
	void SpawnDirtYellow		(const glm::vec2& spawnPos) const;
	void SpawnDirtOrangeLight	(const glm::vec2& spawnPos) const;
	void SpawnDirtOrangeDark	(const glm::vec2& spawnPos) const;
	void SpawnDirtRed			(const glm::vec2& spawnPos) const;
	void SpawnFlower			(const glm::vec2& spawnPos) const;

	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	int m_Player1Health;
	int m_Player2Health;
	int m_PlayerCount{};

	dae::Scene& m_Scene;
	GridComponent*					m_GridComponent{ nullptr };

	std::shared_ptr<SoundHandler>	m_SoundHandler;
	std::shared_ptr<ScoreHandler>	m_ScoreHandler;

	std::unique_ptr<ScoreDisplayer> m_ScoreDisplayer;
};
