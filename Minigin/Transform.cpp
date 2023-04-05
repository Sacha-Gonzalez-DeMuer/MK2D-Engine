#include "Transform.h"
#include "GameObject.h"

dae::Transform::Transform(std::weak_ptr<GameObject> owner)
	: m_owner{ owner }
	, m_localPosition{0}
	, m_isPositionDirty{true}
	, m_worldPosition{0}
{
}

const glm::vec2& dae::Transform::GetWorldPosition()
{
	if (m_isPositionDirty)
		UpdateWorldPosition();

	return m_worldPosition;
}

void dae::Transform::UpdateWorldPosition()
{
	if (m_isPositionDirty)
	{
		if (!m_owner.lock())
			m_worldPosition = m_localPosition;
		else
			m_worldPosition = m_owner.lock()->GetTransform()->GetWorldPosition() + m_localPosition;

		m_isPositionDirty = false;
	}
}

void dae::Transform::SetLocalPosition(float x, float y)
{
	m_localPosition.x = x;
	m_localPosition.y = y;

	SetPositionDirty();
}

void dae::Transform::SetLocalPosition(const glm::vec2& toPos)
{
	m_localPosition = toPos;
	
	SetPositionDirty();
}

void dae::Transform::SetPositionDirty()
{
	m_isPositionDirty = true;

	if (!m_owner.lock()) return;

	const auto& children{ m_owner.lock()->GetChildren() };

	for (auto& child : children)
	{
		if (child)
		{
			child->GetTransform()->SetPositionDirty();
			child->GetTransform()->UpdateWorldPosition();
		}
	}
}

