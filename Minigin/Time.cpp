#include "Time.h"

dae::Time::Time()
	: m_DeltaTime{0}
	, m_TotalTime{0}
{
}

void dae::Time::Update()
{
	const auto currentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float>(currentTime - m_LastTime).count();

	m_TotalTime += m_DeltaTime;
	m_LastTime = currentTime;
}
