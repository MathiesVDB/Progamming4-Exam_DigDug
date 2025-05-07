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

	// -------------------------------------------------
	// Copy/move constructors and assignment operators
	// -------------------------------------------------
	Level(const Level& other)					= default;
	Level(Level&& other) noexcept				= default;
	Level& operator=(const Level& other)		= default;
	Level& operator=(Level&& other) noexcept	= default;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	void LoadLevel(const std::string& fileName); // Will look for file in ../Data/Levels/

private:
	//-------------------------------------------------
	// Private member functions								
	//-------------------------------------------------
	void SpawnPlayer(const Point2f& spawnPos) const;
	void SpawnPooka(const Point2f& spawnPos) const;
	void SpawnFygar(const Point2f& spawnPos) const;
	void SpawnRock(const Point2f& spawnPos) const;
	void SpawnDirtYellow(const Point2f& spawnPos) const;
	void SpawnDirtOrangeLight(const Point2f& spawnPos) const;
	void SpawnDirtOrangeDark(const Point2f& spawnPos) const;
	void SpawnDirtRed(const Point2f& spawnPos) const;
	void SpawnEmpty(const Point2f& spawnPos) const;
	void SpawnFlower(const Point2f& spawnPos) const;

	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	dae::Scene& m_Scene;

};
