#include "Input.h"
#include <algorithm>
#include "imgui_impl_sdl2.h"

void dae::Input::AddController(std::shared_ptr<GenericController> controller)
{
	if (controller->GetControllerIndex() >= m_Controllers.size())
		m_Controllers.resize(controller->GetControllerIndex() + 1);

	m_Controllers[controller->GetControllerIndex()] = controller;
}

std::shared_ptr<dae::GenericController> dae::Input::AddController()
{
	int idx = static_cast<int>(m_Controllers.size());
	auto controller = std::make_shared<GenericController>(idx);
	m_Controllers.push_back(controller);
	return controller;
}

void dae::Input::AddCommand(const ControllerKey& key, std::shared_ptr<Command> command)
{
	m_ControllerCommands[key] = command;
}

void dae::Input::AddCommand(const SDL_Scancode& key, std::shared_ptr<Command> command)
{
	m_KeyboardCommands[key] = command;
}

unsigned int dae::Input::GetNrControllers() const
{
	return static_cast<unsigned int>(m_Controllers.size());
}

void dae::Input::SetControllerActive(unsigned int idx, bool active)
{
	if (idx >= m_Controllers.size() || idx < 0) return;

	m_Controllers[idx]->SetActive(active);
}

bool dae::Input::HandleInput() const
{
	bool exit{ false };

	// update controllers
	for (const auto& controller : m_Controllers)
	{
		if(controller) controller->Update();
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
		auto key = command.first;

		if (pKeys[key])
			command.second->Execute();
	}

	return exit;
}
