#pragma once
#include "Component.h"
#include "GridGraph.h"
#include <string>
namespace dae
{
	class RenderComponent;
	class LevelComponent final : public Component
	{
		using Grid2D = GridGraph<GraphNode2D, GraphConnection2D>;

	public:
		LevelComponent(const std::wstring& levelPath);
		~LevelComponent() = default;

		virtual void Render() const override;

		std::shared_ptr<Grid2D> GetGrid() const { return m_pGridGraph; }

	private:
		std::shared_ptr<Grid2D> m_pGridGraph;
	};
}


