#pragma once
#include <glm/glm.hpp>
namespace dae
{
	namespace MathHelpers
	{
		inline float glmDistanceSquared(const glm::vec2& a, const glm::vec2& b)
		{
			float x = a.x - b.x;
			float y = a.y - b.y;
			return x * x + y * y;
		}
	}
}