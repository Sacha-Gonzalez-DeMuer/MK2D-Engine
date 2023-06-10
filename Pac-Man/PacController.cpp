#include "PacController.h"
#include "PacFilePaths.h"
#include "PacCommands.h"
#include "Input.h"
#include "PacData.h"
#include "ICollider.h"
#include "GameTime.h"
namespace dae
{
	PacController::PacController(std::shared_ptr<PacNavigator> pNavigator)
		: m_pNavigator{pNavigator}
	{
		InitializeInput();
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

	void PacController::InitializeInput()
	{
		// pacman commands
		auto move_right_command{ std::make_shared<dae::PacMoveCommand>(m_pNavigator, Direction::RIGHT) };
		auto move_left_command{ std::make_shared<dae::PacMoveCommand>(m_pNavigator, Direction::LEFT) };
		auto move_up_command{ std::make_shared<dae::PacMoveCommand>(m_pNavigator, Direction::UP) };
		auto move_down_command{ std::make_shared<dae::PacMoveCommand>(m_pNavigator, Direction::DOWN) };

		// keyboard binds
		dae::Input::Get().AddCommand(std::make_pair(SDL_SCANCODE_D, SDL_KEYDOWN), move_right_command);
		dae::Input::Get().AddCommand(std::make_pair(SDL_SCANCODE_A, SDL_KEYDOWN), move_left_command);
		dae::Input::Get().AddCommand(std::make_pair(SDL_SCANCODE_W, SDL_KEYDOWN), move_up_command);
		dae::Input::Get().AddCommand(std::make_pair(SDL_SCANCODE_S, SDL_KEYDOWN), move_down_command);
	}
}

