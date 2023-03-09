#include "Transform.h"
#include "GameObject.h"
#include <iostream>

dae::Transform::Transform(std::weak_ptr<GameObject> owner)
	: m_owner{ owner }
	, m_localPosition{0}
	, m_Dirty{true}
	, m_worldPosition{0}
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

	SetDirty();
}

void dae::Transform::SetLocalPosition(const glm::vec2& toPos)
{
	m_localPosition = toPos;

	SetDirty();
}

void dae::Transform::SetDirty()
{
	m_Dirty = true;

	if (!m_owner.lock()) return;

	const auto& children{ m_owner.lock()->GetChildren() };
	if (children.empty())
	{
		std::cout << "Children empty!\n";
	};

	for (auto& child : children)
	{
		if (child)
		{
			child->GetTransform()->SetDirty();
			child->GetTransform()->UpdateWorldPosition();
		}
	}
}