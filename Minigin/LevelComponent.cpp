#include "LevelComponent.h"
#include "GridGraph.h"
#include "RenderComponent.h"

namespace dae
{
	LevelComponent::LevelComponent(const std::wstring& /*levelPath*/)
		: m_pGridGraph(std::make_unique<GridGraph<GraphNode2D, GraphConnection2D>>(10, 10, 10, false, false))
		//, m_pMapRenderer(std::make_unique<RenderComponent>(levelPath))
	{
	}

	void LevelComponent::Render() const
	{
		//m_pMapRenderer->Render();
	}
}
