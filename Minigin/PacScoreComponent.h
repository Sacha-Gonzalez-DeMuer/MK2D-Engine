#pragma once
#include "Component.h"
#include "Delegate.h"

namespace dae
{
	class PacScoreComponent final : public Component
	{
	public:
		PacScoreComponent(int score) : m_Score{ score } {};
		virtual ~PacScoreComponent() = default;

		void AddScore(int score);
		int GetScore() const { return m_Score; };

		Delegate<> OnScoreChanged;

	private:
		int m_Score;
	};
}
