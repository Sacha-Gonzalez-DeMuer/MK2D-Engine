#include "Orbiter.h"
#include "Transform.h"
#include "Time.h"
#include <iostream>

void dae::Orbiter::Update()
{
	if (m_orbitCenter) m_orbitPosition = m_orbitCenter->GetWorldPosition();

	glm::vec2 target_postion{};

	constexpr float pi{ 3.14159265359f };
	constexpr float to_rad{ pi / 180.0f };
	m_angle = Time::GetInstance().TotalTime() * to_rad * m_orbitSpeed;

	target_postion.x = m_orbitDistance * cosf(m_angle) + m_orbitPosition.x;
	target_postion.y = m_orbitDistance * sinf(m_angle) + m_orbitPosition.y;
	
	m_owner.lock()->GetTransform()->SetLocalPosition(target_postion);
}
