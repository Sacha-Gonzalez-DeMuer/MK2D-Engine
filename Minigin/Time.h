#pragma once
#include "Singleton.h"
#include <chrono>

namespace dae
{
	class Time : public Singleton<Time>
	{
	public:
		Time();

		void Update();

		float DeltaTime() { return m_DeltaTime; };

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_LastTime;

		float m_DeltaTime;
		float m_Time;
	};
}
