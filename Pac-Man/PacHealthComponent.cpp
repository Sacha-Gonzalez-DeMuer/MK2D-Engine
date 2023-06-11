#include "PacHealthComponent.h"
#include <iostream>
#include "ICollider.h"
#include "PacData.h"
#include "GameTime.h"
void dae::PacHealthComponent::Update()
{
	if (m_InvincibilityTimer > 0)
	{
		m_InvincibilityTimer -= GameTime::Get().DeltaTime();
	}
}

void dae::PacHealthComponent::Die()
{
	OnDeath.Invoke();
}

void dae::PacHealthComponent::TakeHit()
{
	if (m_InvincibilityTimer > 0)
		return;

	--m_RemainingLives;

	if (m_RemainingLives < 0)
	{
		m_RemainingLives = 0;
		OnDeath.Invoke();
	}
	else
	{
		OnHitTaken.Invoke();
		m_InvincibilityTimer = m_InvincibilityTime;
	}
}

void dae::PacHealthComponent::OnCollisionEnter(ICollider& other)
{
	if (other.GetOwner()->GetTag() == PacData::PacTags::Ghost)
	{
		TakeHit();
	}
}