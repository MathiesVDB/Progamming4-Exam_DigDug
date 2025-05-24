#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "GridComponent.h"
#include "Component.h"
#include <iostream>
#include "Scene.h"

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
	std::unique_ptr<dae::GameObject> SpawnEmpty(const Point2f& spawnPos) const;

	dae::Scene& GetScene() const { return m_Scene; }

private:
	//-------------------------------------------------
	// Private member functions								
	//-------------------------------------------------
	void SpawnPlayer(const Point2f& spawnPos);
	void SpawnPooka(const Point2f& spawnPos) const;
	void SpawnFygar(const Point2f& spawnPos) const;
	void SpawnRock(const Point2f& spawnPos) const;
	void SpawnDirtYellow(const Point2f& spawnPos) const;
	void SpawnDirtOrangeLight(const Point2f& spawnPos) const;
	void SpawnDirtOrangeDark(const Point2f& spawnPos) const;
	void SpawnDirtRed(const Point2f& spawnPos) const;
	void SpawnFlower(const Point2f& spawnPos) const;

	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	dae::Scene& m_Scene;
	GridComponent* m_GridComponent{ nullptr };
};
