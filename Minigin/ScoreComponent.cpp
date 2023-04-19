#include "ScoreComponent.h"
#include "ICollider.h"

void dae::ScoreComponent::AddScore(int score)
{
	m_Score += score;
	NotifyObservers(m_Score);
};

void dae::ScoreComponent::OnCollision(ICollider& other)
{
	if (other.GetOwner()->GetTag() == "Dot")
	{
		++m_Score;
		NotifyObservers(m_Score);
		other.GetOwner()->Destroy();
	}
}
