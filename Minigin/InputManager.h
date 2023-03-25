#pragma once
#include "Singleton.h"
#include "Datatypes.h"
#include "Keybindings.h"
#include <memory>
namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		SDL_Keycode GetKeyForAction(const KeybindingAction& action) const;
		SDL_Scancode GetScancodeForAction(const KeybindingAction& action) const;

		bool ProcessInput();
		bool IsKeyDown(const SDL_Keycode& keycode) const;

	private:
		std::unique_ptr<Keybindings> m_pKeybindings;
		bool m_IsRebinding;
		SDL_Keycode m_LastKeyDown;
		SDL_Scancode m_LastKey;
	};
}
