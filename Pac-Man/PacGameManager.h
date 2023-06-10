#pragma once
#include <vector>
#include <memory>
#include "PacData.h"
#include "Singleton.h"

namespace dae
{
	class Scene;
	class PacLevel;
	class GameObject;
	class PacSpawner;
	class PacController;
	class PacGameManager final : public Singleton<PacGameManager>
	{
	public:
		PacGameManager() = default;
		void Initialize(const PacData::PacGameData& gameData);

		void LoadGamemodeSelect();
		void LoadLevel(int idx);
		void LoadNextLevel(bool unloadCurrent = false);

	private:
		int m_CurrentLevelIdx{};
		PacData::PacGameData m_GameData;
		std::vector<std::shared_ptr<PacLevel>> m_pLevels{};
		std::vector<std::shared_ptr<GameObject>> m_pPlayers{};

		std::shared_ptr<GameObject> AddPlayer(std::shared_ptr<PacLevel> level, Scene& scene);
		std::shared_ptr<PacSpawner> AddSpawner(std::shared_ptr<PacLevel> level, Scene& scene);
	};
}
