#pragma once
#include "Component.h"
#include "PacData.h"
#include "Delegate.h"

namespace dae
{
	class RenderComponent;
	class PacGrid;
	class GridGraph;
	class PacLevel final : public Component
	{
	public:
		PacLevel() = delete;
		explicit PacLevel(const PacData::PacLevelData& levelData, std::shared_ptr<PacGrid> pacGrid);
		~PacLevel() = default;

		virtual void Start() override;
		virtual void Render() const override;
		virtual void Update() override;

		Delegate<> OnLevelCompleted;

		std::shared_ptr<GridGraph> GetGrid() const;
		std::shared_ptr<PacGrid> GetPacGrid() const { return m_pPacGrid; }
	private:
		PacData::PacLevelData m_LevelData;
		std::shared_ptr<PacGrid> m_pPacGrid;

		bool CheckWinConditions() const;
	};
}