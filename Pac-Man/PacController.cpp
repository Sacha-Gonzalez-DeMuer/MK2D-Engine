#include "PacController.h"
#include "PacFilePaths.h"
#include "PacCommands.h"
#include "Input.h"
#include "PacData.h"
namespace dae
{
	PacController::PacController(std::shared_ptr<PacNavigator> pNavigator)
		: m_pNavigator{pNavigator}
	{
		InitializeInput();
	}

	void PacController::InitializeInput()
	{
		// pacman commands
		auto move_right_command{ std::make_shared<dae::PacMoveCommand>(m_pNavigator, PacData::Direction::RIGHT) };
		auto move_left_command{ std::make_shared<dae::PacMoveCommand>(m_pNavigator, PacData::Direction::LEFT) };
		auto move_up_command{ std::make_shared<dae::PacMoveCommand>(m_pNavigator, PacData::Direction::UP) };
		auto move_down_command{ std::make_shared<dae::PacMoveCommand>(m_pNavigator, PacData::Direction::DOWN) };

		// keyboard binds
		dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_D, SDL_KEYDOWN), move_right_command);
		dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_A, SDL_KEYDOWN), move_left_command);
		dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_W, SDL_KEYDOWN), move_up_command);
		dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_S, SDL_KEYDOWN), move_down_command);
	}
}

