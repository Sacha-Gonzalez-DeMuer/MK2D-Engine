#include "PacScoreComponent.h"
#include "ICollider.h"
#include <fstream>

void dae::PacScoreComponent::AddScore(int score)
{
	m_Score += score;
	OnScoreChanged.Invoke();
}