#include "PlayerController.h"
#include "Commands.h"
#include <memory>
#include "Input.h"
#include "GameObject.h"

dae::PlayerController::PlayerController(std::shared_ptr<GridNavComponent> pNavigator)
	: m_Speed(100.f)
	, m_pNavigator(pNavigator)
{
	InitializeInput();
}

void dae::PlayerController::Update()
{
}

void dae::PlayerController::Render() const
{
}

void dae::PlayerController::InitializeInput()
{
	// pacman commands
	auto move_right_command{ std::make_shared<dae::GridMoveCommand>(m_pNavigator, Direction::RIGHT, m_Speed) };
	auto move_left_command{ std::make_shared<dae::GridMoveCommand>(m_pNavigator, Direction::LEFT, m_Speed) };
	auto move_up_command{ std::make_shared<dae::GridMoveCommand>(m_pNavigator, Direction::UP, m_Speed) };
	auto move_down_command{ std::make_shared<dae::GridMoveCommand>(m_pNavigator, Direction::DOWN, m_Speed) };

	// keyboard binds
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_D, SDL_KEYDOWN), move_right_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_W, SDL_KEYDOWN), move_up_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_A, SDL_KEYDOWN), move_left_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_S, SDL_KEYDOWN), move_down_command);

	// Controller binds
	unsigned int controller_idx = dae::Input::GetInstance().Add360Controller();
	dae::Input::GetInstance().AddCommand(std::make_pair(controller_idx, dae::XBox360Controller::ControllerButton::DPadRight), move_right_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(controller_idx, dae::XBox360Controller::ControllerButton::DPadLeft), move_left_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(controller_idx, dae::XBox360Controller::ControllerButton::DPadUp), move_up_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(controller_idx, dae::XBox360Controller::ControllerButton::DPadDown), move_down_command);
}
