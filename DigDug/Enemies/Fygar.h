#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------


//-----------------------------------------------------
// Fygar Class									 
//-----------------------------------------------------
class Fygar final
{
public:
	Fygar(); // Constructor
	~Fygar(); // Destructor

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	Fygar(const Fygar& other) = default;
	Fygar(Fygar&& other) noexcept = default;
	Fygar& operator=(const Fygar& other) = default;
	Fygar& operator=(Fygar&& other) noexcept = default;

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
