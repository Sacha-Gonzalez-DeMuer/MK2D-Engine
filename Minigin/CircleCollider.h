#pragma once
#include "ICollider.h"

namespace dae
{
	class CircleCollider final : public ICollider
	{
	public:
		CircleCollider(float radius) : m_Radius{ radius } {};
		bool Intersects(std::shared_ptr<ICollider> other) override;
		float GetRadius() const { return m_Radius; }

	private:
		float m_Radius;
	};

}

