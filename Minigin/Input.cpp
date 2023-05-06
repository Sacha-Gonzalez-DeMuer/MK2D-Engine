#include "Input.h"
#include <algorithm>
#include "imgui_impl_sdl2.h"

void dae::Input::AddController(std::shared_ptr<XBox360Controller> controller)
{
	m_Controllers.push_back(controller);
}

unsigned int dae::Input::Add360Controller()
{
	int idx = static_cast<int>(m_Controllers.size()) + 1;
	m_Controllers.push_back(std::make_shared<XBox360Controller>(idx));
	return idx;
}

void dae::Input::AddCommand(const ControllerKey& key, std::shared_ptr<Command> command)
{
	m_ControllerCommands[key] = command;
}

void dae::Input::AddCommand(const KeyboardKey& key, std::shared_ptr<Command> command)
{
	m_KeyboardCommands[key] = command;
}

bool dae::Input::HandleInput() const
{
	bool exit{ false };

	// update controllers
	for (const auto& controller : m_Controllers)
	{
		controller->Update();
	}

	// poll controller commands
	for (const auto& command : m_ControllerCommands)
	{
		auto& controller = m_Controllers[command.first.first];
		auto button = command.first.second;

		if (controller->IsDown(button))
			command.second->Execute();

		if (controller->IsDownThisFrame(button))
			command.second->Execute();

		if (controller->IsReleased(button))
			command.second->Execute();

	}

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) return true;

		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	// poll keyboard commands
	const Uint8* pKeys = SDL_GetKeyboardState(nullptr);
	for (const auto& command : m_KeyboardCommands)
	{
		auto key = command.first.first;

		if (pKeys[key])
			command.second->Execute();
	}

	return exit;
}


