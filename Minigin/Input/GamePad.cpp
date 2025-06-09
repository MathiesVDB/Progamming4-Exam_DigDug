#include <iostream>

#include "Gamepad.h"
#include <SDL.h>
#include <memory>
#include <unordered_set>
#include <unordered_map>

class GamePad::GamepadImpl
{
public:
    explicit GamepadImpl(int controllerIndex)
        : m_ControllerIndex(controllerIndex), m_Controller(nullptr), m_IsConnected(false)
    {
        if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) 
        {
            SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            return;
        }

        if (SDL_IsGameController(m_ControllerIndex)) 
        {
            m_Controller = SDL_GameControllerOpen(m_ControllerIndex);
            m_IsConnected = (m_Controller != nullptr);
            std::cout << "Controller initialized" << std::endl;
        }
    }

    ~GamepadImpl()
    {
        if (m_Controller) SDL_GameControllerClose(m_Controller);
    }

    void Update()
    {
        m_ButtonsPressedThisFrame.clear();
        m_ButtonsReleasedThisFrame.clear();

        if (!m_IsConnected) return;

        m_PreviousButtonState = m_CurrentButtonState;

        for (unsigned char i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
        {
            auto button = static_cast<SDL_GameControllerButton>(i);
            bool isPressed = SDL_GameControllerGetButton(m_Controller, button);
            m_CurrentButtonState[i] = isPressed;

            bool wasPressed = m_PreviousButtonState[i];

            if (isPressed && !wasPressed)
                m_ButtonsPressedThisFrame.insert(i);

            if (!isPressed && wasPressed)
                m_ButtonsReleasedThisFrame.insert(i);
        }
    }

    bool IsButtonDown(unsigned int button) const
    {
        return m_ButtonsPressedThisFrame.count(static_cast<Uint8>(button)) > 0;
    }

    bool IsButtonUp(unsigned int button) const
    {
        return m_ButtonsReleasedThisFrame.count(static_cast<Uint8>(button)) > 0;
    }

    bool IsButtonPressed(unsigned int button) const
    {
        auto it = m_CurrentButtonState.find(static_cast<Uint8>(button));
        return it != m_CurrentButtonState.end() && it->second;
    }

private:
    int m_ControllerIndex;
    SDL_GameController* m_Controller;
    bool m_IsConnected;

    std::unordered_set<Uint8> m_ButtonsPressedThisFrame;
    std::unordered_set<Uint8> m_ButtonsReleasedThisFrame;

    std::unordered_map<Uint8, bool> m_CurrentButtonState;
    std::unordered_map<Uint8, bool> m_PreviousButtonState;
};



GamePad::GamePad(int controllerIndex)
	: m_pImpl(std::make_unique<GamepadImpl>(controllerIndex))
{
}

GamePad::~GamePad() = default;

void GamePad::Update() { m_pImpl->Update(); }
bool GamePad::IsButtonDown(unsigned int button) const { return m_pImpl->IsButtonDown(button); }
bool GamePad::IsButtonUp(unsigned int button) const { return m_pImpl->IsButtonUp(button); }
bool GamePad::IsButtonPressed(unsigned int button) const { return m_pImpl->IsButtonPressed(button); }
