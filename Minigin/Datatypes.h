#pragma once
#include <SDL_keycode.h>

namespace dae
{
	enum class KeybindingAction 
	{ 
		START
	};

	struct Keybinding
	{
		KeybindingAction action;
		SDL_Keycode key;
		SDL_Scancode scan;
	};
}