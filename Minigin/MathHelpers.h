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

		inline glm::vec2 glmLerp(const glm::vec2& from, const glm::vec2& to, float t)
		{
			return ((1 - t) * from) + (t * to);
		}

		inline int GenerateRandomRange(int min, int max)
		{
			return rand() % (max - min + 1) + min;
		}

		inline float fGenerateRandomRange(float min, float max)
		{
			return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
		}

	}
}