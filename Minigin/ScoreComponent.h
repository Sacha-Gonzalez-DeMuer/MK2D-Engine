#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae
{
	class ScoreComponent final : public Component, public Subject<int>
	{
	public:
		ScoreComponent(int score) : m_Score{ score } {};

		void AddScore(int score);

		int GetScore() const { return m_Score; };

	private:
		int m_Score;

		void OnCollision(ICollider& other) override;
	};
}
