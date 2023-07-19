#pragma once
#include <SDL_keycode.h>

namespace dae
{
	enum class Direction
	{
		NONE = 0,
		UP = 1, DOWN = -1,
		RIGHT = 2, LEFT = -2,
	};

	constexpr glm::vec2 g_WindowSize{ 1080.f, 1080.f };
}