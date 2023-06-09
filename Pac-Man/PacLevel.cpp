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
		const float cellSize = static_cast<float>(m_pPacGrid->GetCellSize());
		const float dotSize{ cellSize * .2f };
		const float powerupSize{ cellSize * .3f };

		for (auto pNode : nodes)
		{
			const int nodeIdx = pNode->GetIndex();
			const auto& nodeInfo = m_pPacGrid->GetPacNodeInfo(nodeIdx);

			glm::vec4 cellColor{};
			switch (nodeInfo.type)
			{
			case PacData::PacNodeType::WALL:
				cellColor = PacData::WallColor;
				break;

			default:
				cellColor = PacData::WalkableColor;
			}
			
			Renderer::GetInstance()
				.FillRect
				(pNode->GetPosition() + GetOwner()->GetTransform()->GetWorldPosition(), cellSize, cellSize, cellColor);

			if (nodeInfo.hasItem)
			{
				switch (nodeInfo.type)
				{
				case PacData::PacNodeType::DOT:	
					Renderer::GetInstance()
						.FillCircle
						(pNode->GetPosition() + glm::vec2{ cellSize * .5f, cellSize * .5f }, static_cast<int>(dotSize), PacData::InteractableColor);
					break;

				case PacData::PacNodeType::POWERUP:
					Renderer::GetInstance()
						.FillCircle
						(pNode->GetPosition() + glm::vec2{ cellSize * .5f, cellSize * .5f }, static_cast<int>(powerupSize), PacData::InteractableColor);
					break;
				}
			} 

			//Debug::GetInstance().DrawDebugText(std::to_string(pNode->GetIndex()), pNode->GetPosition());
		}
	}
	std::shared_ptr<GridGraph> PacLevel::GetGrid() const
	{
		return m_pPacGrid;
	}
}
