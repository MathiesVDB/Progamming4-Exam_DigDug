#include "InputManager.h"
#include "Gamepad.h"
#include <SDL.h>
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

class InputManager::InputManagerImpl
{
public:
    InputManagerImpl()
        : m_Gamepad(0) // Controller 0
    {
        std::cout << "InputManager initialized.\n";
    }

    bool ProcessInput()
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
	        if (e.type == SDL_QUIT)
	        {
                std::cout << "Window closed, exiting\n";
                return false;
	        }
        }

        m_Gamepad.Update();

        const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

        for (const auto& [key, commands] : m_KeyboardCommands)
        {
            bool isPressed = keyboardState[key];
            bool wasPressed = m_PreviousKeyState[key];

            for (const auto& [keyState, commandPtr] : commands)
            {
                bool execute = false;

                switch (keyState)
                {
                case KeyState::Pressed:
                    execute = isPressed;
                    break;
                case KeyState::Down:
                    execute = isPressed && !wasPressed;
                    break;
                case KeyState::Up:
                    execute = !isPressed && wasPressed;
                    break;
                }

                if (execute && commandPtr)
                {
                    commandPtr->Execute();
                }
            }

            m_PreviousKeyState[key] = isPressed;
        }


        for (const auto& [button, commands] : m_ControllerCommands)
        {
            bool buttonDown = m_Gamepad.IsButtonDown(button);
            bool buttonPressed = m_Gamepad.IsButtonPressed(button);
            bool buttonUp = m_Gamepad.IsButtonUp(button);

            // Execute commands based on button states
            for (const auto& [keyState, command] : commands)
            {
                bool execute = false;
                switch (keyState)
                {
                case KeyState::Down:
                    execute = buttonDown;
                    break;
                case KeyState::Up:
                    execute = buttonUp;
                    break;
                case KeyState::Pressed:
                    execute = buttonPressed;
                    break;
                }

                if (execute)
                {
                    command->Execute();
                }
            }
        }

        return true;
    }

    void AddCommand(unsigned int key, KeyState state, std::unique_ptr<Command> command)
    {
        m_KeyboardCommands[key].emplace_back(state, std::move(command));
    }

    void AddControllerCommand(unsigned int button, KeyState state, std::unique_ptr<Command> command)
    {
        m_ControllerCommands[button].emplace_back(state, std::move(command));
    }

private:
    std::unordered_map<unsigned int, std::vector<std::pair<KeyState, std::unique_ptr<Command>>>> m_KeyboardCommands;
    std::unordered_map<unsigned int, std::vector<std::pair<KeyState, std::unique_ptr<Command>>>> m_ControllerCommands;
    std::unordered_map<unsigned int, bool> m_PreviousKeyState;
    GamePad m_Gamepad;
};

InputManager::InputManager()
    : m_pImpl(std::make_unique<InputManagerImpl>())
{
}

InputManager::~InputManager() = default;

bool InputManager::ProcessInput() { return m_pImpl->ProcessInput(); }
void InputManager::AddCommand(unsigned int key, KeyState state, std::unique_ptr<Command> command)
{
    m_pImpl->AddCommand(key, state, std::move(command));
}
void InputManager::AddControllerCommand(unsigned int button, KeyState state, std::unique_ptr<Command> command)
{
    m_pImpl->AddControllerCommand(button, state, std::move(command));
}
