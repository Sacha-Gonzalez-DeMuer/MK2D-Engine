#include "Time.h"

dae::Time::Time()
{
}

void dae::Time::Update()
{
	const auto currentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float>(currentTime - m_LastTime).count();

	m_Time += m_DeltaTime;
	m_LastTime = currentTime;
}
