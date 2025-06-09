#include "InputManager.h"
#include <iostream>
#include "Gamepad.h"
#include <SDL.h>
#include <unordered_map>
#include <vector>
#include <memory>

class InputManager::InputManagerImpl
{
public:
    InputManagerImpl()
    {
        //m_Gamepads[0] = std::make_unique<GamePad>(0);

        int numJoysticks = SDL_NumJoysticks();
        for (int counter = 0; counter < numJoysticks; ++counter) 
        {
            if (SDL_IsGameController(counter)) 
            {
                RegisterController(counter);
            }
        }

        std::cout << "[InputManager] Input initialized.\n";
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

        for (auto& [id, gamepad] : m_Gamepads) 
        {
            gamepad->Update();
        }


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

        for (const auto& [controllerId, commandMap] : m_ControllerCommands)
        {
            auto iterator = m_Gamepads.find(controllerId);
            if (iterator == m_Gamepads.end()) continue;

            GamePad* gamepad = iterator->second.get();

            for (const auto& [button, commands] : commandMap) 
            {
                bool buttonDown     = gamepad->IsButtonDown(button);
                bool buttonPressed  = gamepad->IsButtonPressed(button);
                bool buttonUp       = gamepad->IsButtonUp(button);

                for (const auto& [keyState, command] : commands)
                {
                    bool execute = false;
                    switch (keyState)
                	{
                    case KeyState::Down:
                    {
                        execute = buttonDown;
                    	break;
                    }
                    case KeyState::Up:
	                {
		                execute = buttonUp;
                    	break;
	                }
                    case KeyState::Pressed:
	                {
		                execute = buttonPressed;
                    	break;
	                }
                    }

                    if (execute && command) command->Execute();
                }
            }
        }

        return true;
    }

    void AddCommand(unsigned int key, KeyState state, std::unique_ptr<Command> command)
    {
        m_KeyboardCommands[key].emplace_back(state, std::move(command));
    }

    void AddControllerCommand(int controllerIndex, unsigned int button, KeyState state, std::unique_ptr<Command> command)
	{
        m_ControllerCommands[controllerIndex][button].emplace_back(state, std::move(command));
    }

    void ClearCommands()
    {
        //Keep global commands
        for (auto it = m_KeyboardCommands.begin(); it != m_KeyboardCommands.end(); )
        {
            auto& commandList = it->second;
            commandList.erase(
                std::remove_if(
                    commandList.begin(),
                    commandList.end(),
                    [](const std::pair<KeyState, std::unique_ptr<Command>>& pair) 
                    {
                        return !pair.second->IsGlobal();
                    }
                ),
                commandList.end()
            );

            if (commandList.empty())
                it = m_KeyboardCommands.erase(it);
            else
                ++it;
        }

        //There are no global controller commands
        m_ControllerCommands.clear();
    }

    void RegisterController(int controllerIndex)
	{
        m_Gamepads[controllerIndex] = std::make_unique<GamePad>(controllerIndex);
        std::cout << "Controller registered" << std::endl;
    }


private:
    std::unordered_map<unsigned int, std::vector<std::pair<KeyState, std::unique_ptr<Command>>>> m_KeyboardCommands;
    std::unordered_map<int, std::unordered_map<unsigned int, std::vector<std::pair<KeyState, std::unique_ptr<Command>>>>> m_ControllerCommands;
    std::unordered_map<unsigned int, bool> m_PreviousKeyState;
    std::unordered_map<int, std::unique_ptr<GamePad>> m_Gamepads;
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
void InputManager::AddControllerCommand(int controllerIndex, unsigned int button, KeyState state, std::unique_ptr<Command> command)
{
    m_pImpl->AddControllerCommand(controllerIndex, button, state, std::move(command));
}

void InputManager::ClearCommands()
{
    m_pImpl->ClearCommands();
}
