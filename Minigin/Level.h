#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Grid.h"
#include <iostream>

//-----------------------------------------------------
// Level Class									 
//-----------------------------------------------------
class Level final
{
public:
	Level(); 
	~Level(); 

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
	void LoadLevel(const std::string& fileName);

private:
	//-------------------------------------------------
	// Private member functions								
	//-------------------------------------------------
	void SpawnPlayer();
	void SpawnPooka();
	void SpawnFygar();
	void SpawnRock();


	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	std::unique_ptr<Grid> m_GridUPtr;
};
