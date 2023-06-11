#pragma once
#include "Component.h"
#include "Delegate.h"

namespace dae
{
	class PacNPC;
	class PacHealthComponent final : public Component
	{
	public:
		PacHealthComponent(int maxLives) 
			: m_MaxLives{ maxLives }, m_RemainingLives{ maxLives }, m_InvincibilityTime{ 3.f }, m_InvincibilityTimer{ 0.f } {};

		void Update() override;

		void Die();

		int GetRemainingLives() const { return m_RemainingLives; }
		void TakeHit();
		Delegate<> OnHitTaken;
		Delegate<> OnDeath;
		Delegate<PacNPC> OnCollisionWithGhost;

	private:
		int m_RemainingLives;
		int m_MaxLives;
		float m_InvincibilityTime;
		float m_InvincibilityTimer;

		//void OnCollisionEnter(ICollider& other) override;
	};
}