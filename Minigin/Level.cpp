//---------------------------
// Include Files
//---------------------------
#include "Level.h"

//---------------------------
// Constructor & Destructor
//---------------------------
Level::Level()
{
	m_GridUPtr = std::make_unique<Grid>();
}

Level::~Level()
{
	// nothing to destroy
}

void Level::LoadLevel(const std::string& fileName)
{
}

void Level::SpawnPlayer()
{
}

void Level::SpawnPooka()
{
}

void Level::SpawnFygar()
{
}

void Level::SpawnRock()
{
}