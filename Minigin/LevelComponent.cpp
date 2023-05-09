#include "LevelComponent.h"
#include "GridGraph.h"
#include "RenderComponent.h"
#include "GridGraph.h"
#include "Debug.h"

namespace dae
{
	LevelComponent::LevelComponent(const std::wstring& /*levelPath*/)
		: m_pGridGraph(std::make_shared<GridGraph>(10, 10, 50, false, false))
		//, m_pMapRenderer(std::make_unique<RenderComponent>(levelPath))
	{
	}

	void LevelComponent::Render() const
	{
		Debug::GetInstance().DrawGrid(m_pGridGraph.get());
	}
}
