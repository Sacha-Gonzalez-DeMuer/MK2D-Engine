#pragma once
#include "Singleton.h"
#include <chrono>

namespace dae
{
	class GameTime : public Singleton<GameTime>
	{
	public:
		GameTime();

		void Update();

		float TotalTime() { return m_TotalTime; };
		float DeltaTime() { return m_DeltaTime; };
		std::chrono::time_point<std::chrono::high_resolution_clock> LastTime() { return m_LastTime; };

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_LastTime;

		float m_DeltaTime;
		float m_TotalTime;
	};
}
