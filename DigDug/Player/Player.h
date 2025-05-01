#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------


//-----------------------------------------------------
// Player Class									 
//-----------------------------------------------------
class Player final
{
public:
	Player(); // Constructor
	~Player(); // Destructor

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	Player(const Player& other) = default;
	Player(Player&& other) noexcept = default;
	Player& operator=(const Player& other) = default;
	Player& operator=(Player&& other) noexcept = default;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------


private:
	//-------------------------------------------------
	// Private member functions								
	//-------------------------------------------------


	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
};
