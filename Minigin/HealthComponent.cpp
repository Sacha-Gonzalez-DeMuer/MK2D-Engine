#include "HealthComponent.h"
#include <iostream>
#include "ICollider.h"

void dae::HealthComponent::Die()
{
	--m_RemainingLives;
	NotifyObservers(*this);
}

void dae::HealthComponent::OnCollision(ICollider& other)
{
	if (other.GetOwner()->GetTag() == "Ghost")
	{
		Die();
		other.GetOwner()->Destroy();
	}
}



