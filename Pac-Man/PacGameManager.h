#pragma once
#include "Component.h"
#include <vector>
#include <memory>
#include "PacData.h"

namespace dae
{
	class Scene;
	class PacLevel;
	class PacGameManager final : public Component
	{
	public:
		PacGameManager(const PacData::PacGameData& gamedata);
		~PacGameManager() = default;

		void Start() override;

		void LoadLevel(int idx, Scene& toScene);

	private:


		PacData::PacGameData m_GameData;
		std::vector<std::shared_ptr<PacLevel>> m_pLevels{};

		std::shared_ptr<PacLevel> m_pCurrentLevel;
	};
}
