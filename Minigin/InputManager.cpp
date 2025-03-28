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
        if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0)
        {
            std::cerr << "SDL_Init(SDL_INIT_GAMECONTROLLER) failed: " << SDL_GetError() << std::endl;
        }
        else
        {
            if (!SDL_IsGameController(0))
            {
                std::cout << "No game controller detected!" << std::endl;
            }
            else
            {
                std::cout << "Game controller detected!" << std::endl;
            }
        }
    }

    bool ProcessInput()
    {
        SDL_PumpEvents();
        m_Gamepad.Update();

        SDL_Event e;

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                std::cout << "Window closed, exiting\n";
                return false;
            }
        }

        const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

        for (const auto& [key, commands] : m_KeyboardCommands)
        {
            bool isPressed = keyboardState[key];

            for (const auto& [keyState, command] : commands)
            {
                if (keyState == KeyState::Pressed && isPressed)
                {
                    command->Execute();
                }
            }

            m_PreviousKeyState[key] = isPressed;
        }

        for (const auto& [button, commands] : m_ControllerCommands)
        {
            bool buttonDown = m_Gamepad.IsButtonDown(button);
            bool buttonPressed = m_Gamepad.IsButtonPressed(button);
            bool buttonUp = m_Gamepad.IsButtonUp(button);

            // Debugging output for button states
            std::cout << "Button: " << button
                << " Down: " << buttonDown
                << " Pressed: " << buttonPressed
                << " Up: " << buttonUp << std::endl;

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
                    // Debugging message for command execution
                    std::cout << "Executing command for button: " << button << std::endl;
                    command->Execute();
                }
            }
        }

        return true;
    }

    void AddCommand(unsigned int key, KeyState state, std::shared_ptr<Command> command)
    {
        m_KeyboardCommands[key].emplace_back(state, command);
    }

    void AddControllerCommand(unsigned int button, KeyState state, std::shared_ptr<Command> command)
    {
        m_ControllerCommands[button].emplace_back(state, command);
    }

private:
    std::unordered_map<unsigned int, std::vector<std::pair<KeyState, std::shared_ptr<Command>>>> m_KeyboardCommands;
    std::unordered_map<unsigned int, std::vector<std::pair<KeyState, std::shared_ptr<Command>>>> m_ControllerCommands;
    std::unordered_map<unsigned int, bool> m_PreviousKeyState; // Store key states dynamically
    GamePad m_Gamepad;
};

InputManager::InputManager()
    : m_pImpl(std::make_unique<InputManagerImpl>())
{
}

InputManager::~InputManager() = default;

bool InputManager::ProcessInput() { return m_pImpl->ProcessInput(); }
void InputManager::AddCommand(unsigned int key, KeyState state, std::shared_ptr<Command> command)
{
    m_pImpl->AddCommand(key, state, command);
}
void InputManager::AddControllerCommand(unsigned int button, KeyState state, std::shared_ptr<Command> command)
{
    m_pImpl->AddControllerCommand(button, state, command);
}
