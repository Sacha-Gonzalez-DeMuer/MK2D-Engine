#include "Transform.h"
#include "GameObject.h"

dae::Transform::Transform(std::weak_ptr<GameObject> owner)
	: m_owner{ owner }
	, m_localPosition{0}
	, m_Dirty{true}
{
}

const glm::vec2& dae::Transform::GetWorldPosition()
{
	if (m_Dirty)
		UpdateWorldPosition();

	return m_worldPosition;
}

void dae::Transform::UpdateWorldPosition()
{
	if (m_Dirty)
	{
		if (!m_owner.lock())
			m_worldPosition = m_localPosition;
		else
			m_worldPosition = m_owner.lock()->GetTransform()->GetWorldPosition() + m_localPosition;

		m_Dirty = false;
	}
}

void dae::Transform::SetLocalPosition(float x, float y)
{
	m_localPosition.x = x;
	m_localPosition.y = y;

	m_Dirty = true;
}

void dae::Transform::SetLocalPosition(const glm::vec2& toPos)
{
	m_localPosition = toPos;

	m_Dirty = true;
}

