#include "PacHealthComponent.h"
#include <iostream>
#include "ICollider.h"

void dae::PacHealthComponent::Die()
{
	OnDeath.Invoke();
}

void dae::PacHealthComponent::TakeHit()
{
	--m_RemainingLives;

	if (m_RemainingLives <= 0)
		OnDeath.Invoke();
	else
		OnHitTaken.Invoke();
}
