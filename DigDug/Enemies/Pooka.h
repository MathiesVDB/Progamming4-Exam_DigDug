#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------


//-----------------------------------------------------
// Pooka Class									 
//-----------------------------------------------------
class Pooka final
{
public:
	Pooka(); // Constructor
	~Pooka(); // Destructor

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	Pooka(const Pooka& other) = default;
	Pooka(Pooka&& other) noexcept = default;
	Pooka& operator=(const Pooka& other) = default;
	Pooka& operator=(Pooka&& other) noexcept = default;

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
