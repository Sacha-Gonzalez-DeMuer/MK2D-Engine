#include "PacLevel.h"
#include "RenderComponent.h"
#include "Debug.h"
#include "PacGrid.h"
#include "Renderer.h"
#include "GraphNode.h"

namespace dae
{
	PacLevel::PacLevel(const std::vector<std::string>& levelPath)
		: m_pPacGrid(std::make_shared<PacGrid>(levelPath))
	{ }

	void PacLevel::Render() const
	{
		const auto& nodes = m_pPacGrid->GetAllNodes();

		for (auto pNode : nodes)
		{
			const int nodeIdx = pNode->GetIndex();
			glm::vec4 cellColor{};

			switch (m_pPacGrid->GetPacNodeInfo(nodeIdx).type)
			{
			case PacGridData::PacNodeType::Wall:
				cellColor = LevelData::WallColor;
				break;

			default:
				cellColor = LevelData::WalkableColor;
			}

			Renderer::GetInstance()
				.FillRect
				(pNode->GetPosition()
					, static_cast<float>(m_pPacGrid->GetCellSize()), static_cast<float>(m_pPacGrid->GetCellSize())
					, cellColor);

			//Debug::GetInstance().DrawDebugText(std::to_string(pNode->GetIndex()), pNode->GetPosition());
		}
	}
	std::shared_ptr<GridGraph> PacLevel::GetGrid() const
	{
		return m_pPacGrid;
	}
}
