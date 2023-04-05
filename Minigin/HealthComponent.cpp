#include "HealthComponent.h"
#include <iostream>

void dae::HealthComponent::TakeDamage(float damage)
{
	m_CurrentHealth -= damage;
	if (m_CurrentHealth < 0)
	{
		m_CurrentHealth = 0;
		Die();
	}
}

void dae::HealthComponent::Heal(float amount)
{
	m_CurrentHealth += amount;
	if(m_CurrentHealth > m_MaxHealth)
		m_CurrentHealth = m_MaxHealth;
}

void dae::HealthComponent::InstaKill()
{
	m_CurrentHealth = 0;
	Die();
}

void dae::HealthComponent::Die()
{
	std::cout << "died" << std::endl;
	--m_RemainingLives;
}
