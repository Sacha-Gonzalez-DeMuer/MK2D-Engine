#include "Transform.h"

dae::Transform::Transform()
	: m_needsUpdate{ false }
	, m_parent{ nullptr }
	, m_position{0,0,0}
{
}

dae::Transform::Transform(const glm::vec3& pos)
	: m_needsUpdate{false}
	, m_parent{nullptr}
	, m_position{pos}
{
}

const glm::vec3& dae::Transform::GetWorldPosition() const
{
	if (!m_parent) return m_position;

	return (m_parent->GetWorldPosition() + GetLocalPosition());
}

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}
