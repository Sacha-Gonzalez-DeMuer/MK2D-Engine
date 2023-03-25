#include <SDL.h>
#include "InputManager.h"
#include "imgui_impl_sdl2.h"
#include <stdexcept>

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
		}
		// etc...


		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}

bool dae::InputManager::IsKeyDown(const SDL_Keycode& keycode) const
{
	const Uint8* state = SDL_GetKeyboardState(nullptr);
	return state[SDL_GetScancodeFromKey(keycode)];
}

SDL_Keycode dae::InputManager::GetKeyForAction(const KeybindingAction& action) const
{
	for (const auto& keybindings : m_pKeybindings->GetKeybindings()) {
		if (keybindings.action == action) {
			return keybindings.key;
		}
	}

	throw std::runtime_error("Could not find keybinding for action");
}

SDL_Scancode dae::InputManager::GetScancodeForAction(const KeybindingAction& action) const
{
	for (const auto& keybindings : m_pKeybindings->GetKeybindings()) {
		if (keybindings.action == action) {
			return keybindings.scan;
		}
	}

	throw std::runtime_error("Could not find keybinding for action");
}
