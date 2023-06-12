#include "PacController.h"
#include "PacFilePaths.h"
#include "PacCommands.h"
#include "Input.h"
#include "PacData.h"
#include "ICollider.h"
#include "GameTime.h"
#include "GenericController.h"

namespace dae
{
	PacController::PacController(std::shared_ptr<PacNavigator> pNavigator, int playerIdx)
		: m_pNavigator{ pNavigator }, m_PlayerIdx{ playerIdx }, m_PowerUpTimer{ 0.f }
	{
		InitializeInput(playerIdx);
	}

	void PacController::PowerUp(float duration)
	{
		m_PowerUpTimer = duration;
		OnPowerup.Invoke(duration);
	}

	void PacController::Update()
	{
		float dt = GameTime::Get().DeltaTime();

		if (m_PowerUpTimer > 0.f)
		{
			m_PowerUpTimer -= dt;
			if (m_PowerUpTimer <= 0.f) m_PowerUpTimer = 0.f;
		}
	}

	void PacController::SetGenericControllerActive(bool active)
	{
		m_pGenericController->SetActive(active);
	}

	void PacController::InitializeInput(unsigned int playerIdx)
	{
		auto move_right_command{ std::make_shared<dae::PacMoveCommand>(m_pNavigator, Direction::RIGHT) };
		auto move_left_command{ std::make_shared<dae::PacMoveCommand>(m_pNavigator, Direction::LEFT) };
		auto move_up_command{ std::make_shared<dae::PacMoveCommand>(m_pNavigator, Direction::UP) };
		auto move_down_command{ std::make_shared<dae::PacMoveCommand>(m_pNavigator, Direction::DOWN) };

		if (playerIdx == 0)
		{
			dae::Input::Get().AddCommand(SDL_SCANCODE_D, move_right_command);
			dae::Input::Get().AddCommand(SDL_SCANCODE_A, move_left_command);
			dae::Input::Get().AddCommand(SDL_SCANCODE_W, move_up_command);
			dae::Input::Get().AddCommand(SDL_SCANCODE_S, move_down_command);
		}

		m_pGenericController = std::make_shared<GenericController>(playerIdx);
		dae::Input::Get().AddController(m_pGenericController);
		dae::Input::Get().AddCommand(std::make_pair(m_pGenericController->GetControllerIndex(), GenericController::ControllerButton::DPadRight), move_right_command);
		dae::Input::Get().AddCommand(std::make_pair(m_pGenericController->GetControllerIndex(), GenericController::ControllerButton::DPadLeft), move_left_command);
		dae::Input::Get().AddCommand(std::make_pair(m_pGenericController->GetControllerIndex(), GenericController::ControllerButton::DPadUp), move_up_command);
		dae::Input::Get().AddCommand(std::make_pair(m_pGenericController->GetControllerIndex(), GenericController::ControllerButton::DPadDown), move_down_command);
	}
}

