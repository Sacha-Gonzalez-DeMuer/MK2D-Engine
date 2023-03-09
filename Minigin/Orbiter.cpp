#include "Orbiter.h"
#include "Transform.h"
#include "Time.h"
#include <iostream>

void dae::Orbiter::Update()
{
	if (m_owner->GetParent().expired()) return;

	glm::vec2 target_postion{};

	constexpr float pi{ 3.14159265359f };
	constexpr float to_rad{ pi / 180.0f };
	m_angle = Time::GetInstance().TotalTime() * to_rad * m_orbitSpeed;

	target_postion.x = m_orbitDistance * cosf(m_angle) + m_owner->GetParent().lock()->GetTransform()->GetWorldPosition().x;
	target_postion.y = m_orbitDistance * sinf(m_angle) + m_owner->GetParent().lock()->GetTransform()->GetWorldPosition().y;
	
	m_owner->GetTransform()->SetLocalPosition(target_postion);
}
 