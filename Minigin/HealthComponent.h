#pragma once
#include "Component.h"
#include "ISubject.h"

namespace dae
{
	class HealthComponent final : public Component, public ISubject<HealthComponent>
	{
	public:
		HealthComponent(float maxHealth, int maxLives) : m_MaxLives{ maxLives }, m_RemainingLives{ maxLives },  m_MaxHealth { maxHealth }, m_CurrentHealth{ maxHealth } {};

		void TakeDamage(float damage);
		void Heal(float amount);
		void InstaKill();
		void Die();

		float GetCurrentHealth() const { return m_CurrentHealth; }
		int GetRemainingLives() const { return m_RemainingLives; }

	private:
		float m_CurrentHealth;
		float m_MaxHealth;

		int m_RemainingLives;
		int m_MaxLives;
	};
}

