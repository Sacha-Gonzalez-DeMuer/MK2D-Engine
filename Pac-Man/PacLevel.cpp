#include "PacLevel.h"
#include "RenderComponent.h"
#include "Debug.h"
#include "PacGrid.h"
#include "Renderer.h"
#include "GraphNode.h"
#include "PacData.h"
namespace dae
{
	PacLevel::PacLevel(const PacData::PacLevelData& levelData)
		: m_pPacGrid(std::make_shared<PacGrid>(levelData.map))
		, m_LevelData{ levelData }
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
			case PacData::PacNodeType::WALL:
				cellColor = PacData::WallColor;
				break;

			default:
				cellColor = PacData::WalkableColor;
			}

			Renderer::GetInstance()
				.FillRect
				(pNode->GetPosition()
					, static_cast<float>(m_pPacGrid->GetCellSize()), static_cast<float>(m_pPacGrid->GetCellSize())
					, cellColor);

			Debug::GetInstance().DrawDebugText(std::to_string(pNode->GetIndex()), pNode->GetPosition());
		}
	}
	std::shared_ptr<GridGraph> PacLevel::GetGrid() const
	{
		return m_pPacGrid;
	}
}
