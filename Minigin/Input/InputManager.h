#pragma once
#include <memory>

#include "Singleton.h"
#include "Command.h"

enum class KeyState
{
	Down,
	Up,
	Pressed
};

class InputManager final : public dae::Singleton<InputManager>
{
public:
	InputManager();
	~InputManager();

	bool ProcessInput();

	void AddCommand(unsigned int key, KeyState state, std::unique_ptr<Command> command);
	void AddControllerCommand(unsigned int button, KeyState state, std::unique_ptr<Command> command);

	void ClearCommands();

private:
	class InputManagerImpl;
	std::unique_ptr<InputManagerImpl> m_pImpl;
};
