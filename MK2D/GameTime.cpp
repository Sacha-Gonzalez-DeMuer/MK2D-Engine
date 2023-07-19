#include "GameTime.h"
#include <iostream>


dae::GameTime::GameTime()
	: m_DeltaTime{ 0 }
	, m_TotalTime{ 0 }
	, m_LastTime{ std::chrono::high_resolution_clock::now() }
{
}

void dae::GameTime::Update()
{
	const auto currentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float>(currentTime - m_LastTime).count();

	m_TotalTime += m_DeltaTime;
	m_LastTime = currentTime;
}
