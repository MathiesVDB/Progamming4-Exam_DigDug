#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include <memory>

//-----------------------------------------------------
// GamePad Class									 
//-----------------------------------------------------
class GamePad final
{
public:
	explicit GamePad(int controllerIndex = 0);
	~GamePad();

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	GamePad(const GamePad& other) = default;
	GamePad(GamePad&& other) noexcept = default;
	GamePad& operator=(const GamePad& other) = default;
	GamePad& operator=(GamePad&& other) noexcept = default;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	void Update();
	bool IsButtonDown(unsigned int button) const;
	bool IsButtonUp(unsigned int button) const;
	bool IsButtonPressed(unsigned int button) const;

private:
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	class GamepadImpl;
	std::unique_ptr<GamepadImpl> m_pImpl;
};
