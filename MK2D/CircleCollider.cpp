#include "CircleCollider.h"

bool dae::CircleCollider::Intersects(std::shared_ptr<ICollider> other)
{
	std::shared_ptr<CircleCollider> circleCollider = std::dynamic_pointer_cast<CircleCollider>(other);

	if (circleCollider)
	{
		auto thisPos = GetTransform()->GetWorldPosition();
		auto otherPos = other->GetTransform()->GetWorldPosition();

		float dx = thisPos.x - otherPos.x;
		float dy = thisPos.y - otherPos.y;
		float distanceSquared = dx * dx + dy * dy;

		float sumRadiiSquared = (m_Radius + circleCollider->GetRadius()) * (m_Radius + circleCollider->GetRadius());

		return distanceSquared < sumRadiiSquared;
	}

	return false;
}
