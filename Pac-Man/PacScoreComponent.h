#pragma once
#include "Component.h"
#include "Delegate.h"
#include "PacData.h"
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

		void SerializeHighscore(const PacData::PacPlayerInfo& playerInfo, const std::string& fileName);
	};
}
