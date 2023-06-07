#pragma once
#include "Component.h"

namespace dae
{
	class RenderComponent;
	class PacGrid;
	class GridGraph;
	class PacLevel final : public Component
	{
	public:
		explicit PacLevel(const std::vector<std::string>& levelPath);
		~PacLevel() = default;

		virtual void Render() const override;

		std::shared_ptr<GridGraph> GetGrid() const;
		std::shared_ptr<PacGrid> GetPacGrid() const { return m_pPacGrid; }
	private:
		std::shared_ptr<PacGrid> m_pPacGrid;
	};
}