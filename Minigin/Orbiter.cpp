#include "Orbiter.h"
#include "Transform.h"
#include "GameTime.h"
#include <iostream>

void dae::Orbiter::Update()
{
	if (GetOwner()->GetParent().expired()) return;

	glm::vec2 target_postion{};

	constexpr float pi{ 3.14159265359f };
	constexpr float to_rad{ pi / 180.0f };
	m_angle = GameTime::Get().TotalTime() * to_rad * m_orbitSpeed;

	target_postion.x = m_orbitDistance * cosf(m_angle) + GetOwner()->GetParent().lock()->GetTransform()->GetWorldPosition().x;
	target_postion.y = m_orbitDistance * sinf(m_angle) + GetOwner()->GetParent().lock()->GetTransform()->GetWorldPosition().y;
	
	GetOwner()->GetTransform()->SetLocalPosition(target_postion);
}
 