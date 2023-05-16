#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae
{
	class HealthComponent final : public Component, public Subject<HealthComponent>
	{
	public:
		HealthComponent(int maxLives) : m_MaxLives{ maxLives }, m_RemainingLives{ maxLives } {};

		void Die();

		int GetRemainingLives() const { return m_RemainingLives; }
	private:
		int m_RemainingLives;
		int m_MaxLives;

		void OnCollision(ICollider& other) override;
	};
}