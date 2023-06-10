#include "PacLevel.h"
#include "RenderComponent.h"
#include "Debug.h"
#include "PacGrid.h"
#include "Renderer.h"
#include "GraphNode.h"
#include "PacData.h"
#include "Datatypes.h"
namespace dae
{
	PacLevel::PacLevel(const PacData::PacLevelData& levelData, std::shared_ptr<PacGrid> pacGrid)
		: m_pPacGrid(pacGrid)
		, m_LevelData{ levelData }
	{ 
	}

	void PacLevel::Start()
	{
		float gridWidth = static_cast<float>(m_pPacGrid->GetCellSize() * m_pPacGrid->GetRows());
		float gridHeight = static_cast<float>(m_pPacGrid->GetCellSize() * m_pPacGrid->GetColumns());
		GetOwner()->GetTransform()->SetLocalPosition(g_WindowSize.x*.5f - gridWidth * .5f, g_WindowSize.y *.5f- gridHeight * .5f);
	}

	void PacLevel::Render() const
	{
		const auto& nodes = m_pPacGrid->GetAllNodes();
		const float cell_size = static_cast<float>(m_pPacGrid->GetCellSize());
		const float dot_size{ cell_size * .2f };
		const float powerup_size{ cell_size * .3f };

		for (auto pNode : nodes)
		{
			const int node_idx = pNode->GetIndex();
			const auto& node_info = m_pPacGrid->GetPacNodeInfo(node_idx);

			glm::vec4 cell_color{};
			switch (node_info.type)
			{
			case PacData::PacNodeType::WALL:
				cell_color = PacData::WallColor;
				break;

			default:
				cell_color = PacData::WalkableColor;
			}
			
			const auto& node_pos = m_pPacGrid->GetNodePos(pNode);
			Renderer::Get()
				.FillRect
				(node_pos, cell_size, cell_size, cell_color);



			if (node_info.hasItem)
			{
				switch (node_info.type)
				{
				case PacData::PacNodeType::DOT:	
					Renderer::Get()
						.FillCircle
						(node_pos + glm::vec2{ cell_size * .5f, cell_size * .5f }, static_cast<int>(dot_size), PacData::InteractableColor);
					break;

				case PacData::PacNodeType::POWERUP:
					Renderer::Get()
						.FillCircle
						(node_pos + glm::vec2{ cell_size * .5f, cell_size * .5f }, static_cast<int>(powerup_size), PacData::InteractableColor);
					break;
				}
			} 

			//Debug::GetInstance().DrawDebugText(std::to_string(pNode->GetIndex()), pNode->GetPosition());
		}
	}

	void PacLevel::Update()
	{
		if (CheckWinConditions())
			OnLevelCompleted.Invoke();
	}

	std::shared_ptr<GridGraph> PacLevel::GetGrid() const
	{
		return m_pPacGrid;
	}

	bool PacLevel::CheckWinConditions() const
	{
		const auto& nodesInfo = m_pPacGrid->GetPacNodeInfo();

		for (const auto& nodeInfo : nodesInfo)
		{
			if ((nodeInfo.type == PacData::PacNodeType::DOT || nodeInfo.type == PacData::PacNodeType::POWERUP)
				&& nodeInfo.hasItem)
				return false;
		}

		return true;
	}
}
