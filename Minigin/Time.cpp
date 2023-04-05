#include "Time.h"
#include <iostream>
dae::Time::Time()
	: m_DeltaTime{ 0 }
	, m_TotalTime{ 0 }
	, m_LastTime{ std::chrono::high_resolution_clock::now() }
{
}

void dae::Time::Update()
{
	const auto currentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float>(currentTime - m_LastTime).count();

	m_TotalTime += m_DeltaTime;
	m_LastTime = currentTime;
}
