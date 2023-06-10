#pragma once
#include <vector>
#include <memory>
#include "PacData.h"
#include "Singleton.h"

namespace dae
{
	class Scene;
	class PacLevel;
	class PacController;
	class PacGameManager final : public Singleton<PacGameManager>
	{
	public:
		PacGameManager() = default;
		void Initialize(const PacData::PacGameData& gameData);

		void LoadLevel(int idx);
		void LoadNextLevel(bool unloadCurrent = false);

	private:
		int m_CurrentLevelIdx{};
		PacData::PacGameData m_GameData;
		std::vector<std::shared_ptr<PacLevel>> m_pLevels{};
	};
}
