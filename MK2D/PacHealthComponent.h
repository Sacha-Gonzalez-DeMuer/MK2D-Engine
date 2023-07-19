#pragma once
#include "Component.h"
#include "Delegate.h"

namespace dae
{
	class PacHealthComponent final : public Component
	{
	public:
		PacHealthComponent(int maxLives) : m_MaxLives{ maxLives }, m_RemainingLives{ maxLives } {};

		void Die();

		int GetRemainingLives() const { return m_RemainingLives; }
		void TakeHit();
		Delegate<> OnHitTaken;
		Delegate<> OnDeath;

	private:
		int m_RemainingLives;
		int m_MaxLives;
	};
}