#include "Gamepad.h"
#include <Windows.h>
#include <Xinput.h>
#include <memory>

#pragma comment(lib, "xinput.lib")

class GamePad::GamepadImpl
{
public:
	explicit GamepadImpl(int controllerIndex)
		: m_ControllerIndex(controllerIndex), m_IsConnected(false)
	{
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
		m_ButtonsPressedThisFrame = 0;
		m_ButtonsReleasedThisFrame = 0;
	}

	void Update()
	{
		m_PreviousState = m_CurrentState;
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));

		// Check if controller is connected
		DWORD result = XInputGetState(m_ControllerIndex, &m_CurrentState);
		m_IsConnected = (result == ERROR_SUCCESS);

		if (!m_IsConnected)
		{
			// Controller is disconnected, reset state
			ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
			m_ButtonsPressedThisFrame = 0;
			m_ButtonsReleasedThisFrame = 0;
			return;
		}

		// Calculate button changes
		auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
		m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
	}

	bool IsButtonDown(unsigned int button) const { return m_IsConnected && (m_ButtonsPressedThisFrame & button); }
	bool IsButtonUp(unsigned int button) const { return m_IsConnected && (m_ButtonsReleasedThisFrame & button); }
	bool IsButtonPressed(unsigned int button) const { return m_IsConnected && (m_CurrentState.Gamepad.wButtons & button); }

private:
	int m_ControllerIndex{};
	XINPUT_STATE m_CurrentState{};
	XINPUT_STATE m_PreviousState{};
	unsigned int m_ButtonsPressedThisFrame{};
	unsigned int m_ButtonsReleasedThisFrame{};
	bool m_IsConnected; // New member to track connection status
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
