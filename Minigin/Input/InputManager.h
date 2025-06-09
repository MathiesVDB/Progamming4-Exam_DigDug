#pragma once
#include <memory>
#include "Singleton.h"

enum class KeyState
{
	Down,
	Up,
	Pressed
};


class Command
{
public:
	virtual ~Command() = default;
	virtual void Execute() = 0;


	void SetGlobal() { m_IsGlobal = true; }
	bool IsGlobal() const { return m_IsGlobal; }

private:
	bool m_IsGlobal{ false };
};

class InputManager final : public dae::Singleton<InputManager>
{
public:
	InputManager();
	~InputManager();

	bool ProcessInput();

	void AddCommand(unsigned int key, KeyState state, std::unique_ptr<Command> command);
	void AddControllerCommand(int controllerIndex, unsigned int button, KeyState state, std::unique_ptr<Command> command);

	void ClearCommands();

private:
	class InputManagerImpl;
	std::unique_ptr<InputManagerImpl> m_pImpl;
};
