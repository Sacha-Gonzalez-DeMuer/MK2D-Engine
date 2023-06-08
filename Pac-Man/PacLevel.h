#pragma once
#include "Component.h"
#include "PacData.h"
namespace dae
{
	class RenderComponent;
	class PacGrid;
	class GridGraph;
	class PacLevel final : public Component
	{
	public:
		PacLevel() = delete;
		explicit PacLevel(const PacData::PacLevelData& levelData);
		~PacLevel() = default;

		virtual void Render() const override;

		std::shared_ptr<GridGraph> GetGrid() const;
		std::shared_ptr<PacGrid> GetPacGrid() const { return m_pPacGrid; }
	private:
		PacData::PacLevelData m_LevelData;
		std::shared_ptr<PacGrid> m_pPacGrid;
	};
}