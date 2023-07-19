#include "PacScoreComponent.h"
#include "ICollider.h"

void dae::PacScoreComponent::AddScore(int score)
{
	m_Score += score;
	OnScoreChanged.Invoke();
};
