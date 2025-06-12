#pragma once
#include "GameDirector.h"
#include "Transform.h"
#include "GameObject.h"
#include "HighScore.h"
#include "InputManager.h"
#include "Player.h"
#include "TextureComponent.h"
#include "SpriteComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ServiceLocator.h"
#include "Fygar.h"

//------------------------------------------------
// Current Commands
//------------------------------------------------

class MoveCommand final : public Command
{
public:
	explicit MoveCommand(dae::GameObject* owner, MoveDirection direction, float movementSpeed, bool isEnemy = false)
		: m_Owner(owner), m_Direction(direction), m_Player{ nullptr }, m_Fygar{ nullptr }, m_IsEnemy{ isEnemy }, m_MovementSpeed{ movementSpeed }
	{
		if (!m_IsEnemy)
		{
			if (owner->HasComponent<Player>()) m_Player = owner->GetComponent<Player>();
			else std::cout << "'Player' component required to check gridmovement!\n";
		}
		else if (owner->HasComponent<Fygar>())
		{
			m_Fygar = owner->GetComponent<Fygar>();
		}
	}

	void Execute() override
	{
		//Enemy will always move straight to a target so checks don't need to happen 
		if (!m_IsEnemy)
		{
			if (!m_Player) return;
			if (m_Player->IsDead()) return;
			if (!m_Player->CanSwitchMovement(m_Direction)) return;
		}
		else if (m_Fygar)
		{
			if (!m_Fygar->CanMove()) return;
			if (!m_Fygar->CanSwitchMovement(m_Direction)) return;
		}

		switch (m_Direction)
		{
		case MoveDirection::Left:
			m_Owner->SetVelocity({ -m_MovementSpeed, 0.f });
			break;
		case MoveDirection::Right:
			m_Owner->SetVelocity({ +m_MovementSpeed, 0.f });
			break;
		case MoveDirection::Up:
			m_Owner->SetVelocity({ 0.f, -m_MovementSpeed });
			break;
		case MoveDirection::Down:
			m_Owner->SetVelocity({ 0.f, +m_MovementSpeed });
			break;
		}
	}

private:
	dae::GameObject*	m_Owner;
	MoveDirection		m_Direction;
	Player*				m_Player;
	Fygar*				m_Fygar;

	float m_MovementSpeed{};

	bool m_IsEnemy{ false };
};

class AttackCommand final : public Command
{
public:
	explicit AttackCommand(dae::GameObject* owner)
		: m_Owner(owner)
	{
		if (owner->HasComponent<Player>()) m_Player = owner->GetComponent<Player>();
		else if (owner->HasComponent<Fygar>()) m_Fygar = owner->GetComponent<Fygar>();
		else std::cout << "'Player' or 'Fygar' component required to attack!\n";
	}

	void Execute() override
	{
		if (m_Fygar != nullptr)
		{
			m_Fygar->Attack();
			return;
		}

		if (m_Player->IsDead()) return;

		m_Player->Attack();
	}

private:
	dae::GameObject* m_Owner;
	Player* m_Player{ nullptr };
	Fygar*  m_Fygar { nullptr };
};

class LetterUpCommand : public Command
{
public:
	explicit LetterUpCommand(HighScore* highScore) : m_HighScore(highScore) {}

	void Execute() override
	{
		auto& letter = m_HighScore->GetLetters()[m_HighScore->GetLetterIndex()];
		letter = (letter == 'Z') ? 'A' : letter + 1;
		m_HighScore->SetCurrentLetter(letter);
		m_HighScore->UpdateLetterDisplay();
	}
private:
	HighScore* m_HighScore;
};

class LetterDownCommand : public Command
{
public:
	explicit LetterDownCommand(HighScore* highScore) : m_HighScore(highScore) {}

	void Execute() override
	{
		auto& letter = m_HighScore->GetLetters()[m_HighScore->GetLetterIndex()];
		letter = (letter == 'A') ? 'Z' : letter - 1;
		m_HighScore->SetCurrentLetter(letter);
		m_HighScore->UpdateLetterDisplay();
	}
private:
	HighScore* m_HighScore;
};

class LetterConfirmCommand : public Command
{
public:
	explicit LetterConfirmCommand(HighScore* highScore)
		: m_HighScore(highScore)
	{
	}

	void Execute() override
	{
		m_HighScore->IncreaseLetterIndex();

		if (m_HighScore->GetLetterIndex() >= 3)
		{
			m_HighScore->FinishEntry();
			return;
		}

		m_HighScore->UpdateLetterDisplay();
	}

private:
	HighScore* m_HighScore;
};

class ButtonUpCommand : public Command
{
public:
	explicit ButtonUpCommand() {}

	void Execute() override
	{
		auto& director = GameDirector::GetInstance();
		if (director.GetSelectedMenuIndex() > 0)
		{
			director.DecreaseMenuIndex();
			director.UpdateMenuVisuals();
		}
	}
};

class ButtonDownCommand : public Command
{
public:
	explicit ButtonDownCommand() {}

	void Execute() override
	{
		auto& director = GameDirector::GetInstance();
		if (director.GetSelectedMenuIndex() < static_cast<int>(director.GetMenuOptions().size()) - 1)
		{
			director.IncreaseMenuIndex();
			director.UpdateMenuVisuals();
		}
	}
};

class ButtonConfirmCommand : public Command
{
public:
	explicit ButtonConfirmCommand()
	{
	}

	void Execute() override
	{
		auto& director = GameDirector::GetInstance();
		director.DetermineGameFlow();
	}
};

class MuteCommand : public Command
{
public:
	explicit MuteCommand()
	{
	}

	void Execute() override
	{
		ServiceLocator::GetSoundSystem().ToggleMute();

		std::cout << "[GLOBAL COMMAND] GAME MUTED" << std::endl;
	}
};

class SkipLevelCommand : public Command
{
public:
	explicit SkipLevelCommand()
	{
	}

	void Execute() override
	{
		GameDirector::GetInstance().SwitchToNextScene();

		std::cout << "[GLOBAL COMMAND] LEVEL SKIPPED" << std::endl;
	}
};