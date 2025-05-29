#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GridComponent.h"
#include "Component.h"
#include "Scene.h"

class RopeComponent;
//-----------------------------------------------------
// Level Class									 
//-----------------------------------------------------
class Level final : public dae::Component
{
public:
	Level(dae::GameObject* owner, const std::string& sceneName);

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	void LoadLevel(const std::string& fileName); // Will look for file in ../Data/Levels/

	//Public because used in player
	std::unique_ptr<dae::GameObject> SpawnEmpty(const glm::vec2& spawnPos) const;
	std::unique_ptr<dae::GameObject> SpawnRope(const glm::vec2& spawnPos) const;

	dae::Scene& GetScene() const { return m_Scene; }

private:
	//-------------------------------------------------
	// Private member functions								
	//-------------------------------------------------
	void SpawnPlayer(const glm::vec2& spawnPos);
	void SpawnPooka(const glm::vec2& spawnPos)			 const;
	void SpawnFygar(const glm::vec2& spawnPos)			 const;
	void SpawnRock(const glm::vec2& spawnPos)			 const;
	void SpawnDirtYellow(const glm::vec2& spawnPos)		 const;
	void SpawnDirtOrangeLight(const glm::vec2& spawnPos) const;
	void SpawnDirtOrangeDark(const glm::vec2& spawnPos)	 const;
	void SpawnDirtRed(const glm::vec2& spawnPos)		 const;
	void SpawnFlower(const glm::vec2& spawnPos)			 const;

	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	dae::Scene& m_Scene;
	GridComponent* m_GridComponent{ nullptr };
};
