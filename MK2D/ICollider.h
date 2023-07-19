#pragma once
#include "Component.h"

namespace dae
{
	class ICollider : public Component
	{
    public:
        ICollider() = default;
        virtual ~ICollider() = default;

        // Check if this collider intersects with another collider
        virtual bool Intersects(std::shared_ptr<ICollider> other) = 0;

        std::shared_ptr<Transform> GetTransform() { return m_gameObject.lock()->GetTransform(); }
	};
}


