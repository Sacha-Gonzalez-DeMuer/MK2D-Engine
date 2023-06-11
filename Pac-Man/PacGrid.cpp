#include "PacGrid.h"
#include "PacData.h"
#include "GraphNode.h"
#include "GraphConnection.h"
#include "MathHelpers.h"
#include <cctype>

dae::PacGrid::PacGrid(const std::vector<std::string>& levelData)
	: GridGraph(static_cast<int>(levelData.size()), static_cast<int>(levelData[0].size()), 40, false, false)
	, m_NodeInfoMap(21*21), m_totalDots{0}, m_totalPowerups{0}
{
	Initialize(levelData);
	UpdateConnections();
}

const PacData::PacNodeInfo& dae::PacGrid::GetPacNodeInfo(int nodeIdx) const
{
	return m_NodeInfoMap[nodeIdx];
}

void dae::PacGrid::SetPacNodeInfo(PacData::PacNodeInfo nodeInfo, int nodeIdx)
{
	m_NodeInfoMap[nodeIdx] = nodeInfo;
}

void dae::PacGrid::SetPacNodeInfo(int idx, PacData::PacNodeType type, bool hasItem)
{
	m_NodeInfoMap[idx].type = type;
	m_NodeInfoMap[idx].hasItem = hasItem;
}

int dae::PacGrid::GetRandomWalkableNodeIdx() const
{
	const int nr_nodes{ static_cast<int>(m_NodeInfoMap.size()) - 1 };
	int rnd{ MathHelpers::GenerateRandomRange(0, nr_nodes) };

	while (m_NodeInfoMap[rnd].type == PacData::PacNodeType::WALL || m_NodeInfoMap[rnd].type == PacData::PacNodeType::NPC_SPAWN)
		rnd = MathHelpers::GenerateRandomRange(0, nr_nodes);
	
	return rnd;
}

const glm::vec2& dae::PacGrid::GetSpawnPos() const
{
	return GetNode(m_PacManSpawnNodeIdx)->GetLocalPosition();
}

glm::vec2 dae::PacGrid::GetNodePos(GraphNode* pNode) const
{
	return pNode->GetLocalPosition() + GetOwner()->GetTransform()->GetWorldPosition();
}

int dae::PacGrid::GetNodeIdxAtWorldPos(const glm::vec2& worldPos) const
{
	int idx = invalid_node_index;
	float cell_size = static_cast<float>(GetCellSize());
	float halfCellSize = cell_size * .5f;
	glm::vec2 position{ worldPos };
	glm::vec2 grid_local_pos = GetOwner()->GetTransform()->GetLocalPosition();

	position.x -= grid_local_pos.x - halfCellSize;
	position.y -= grid_local_pos.y - halfCellSize;


	if (position.x < 0 || position.y < 0)
	{
		return idx;
	}

	int r, c;

	r = static_cast<int>(position.x / cell_size);
	c = static_cast<int>(position.y / cell_size);

	if (!IsWithinBounds(c, r))
		return idx;

	return GetIndex(c, r);
}

void dae::PacGrid::Initialize(const std::vector<std::string>& levelData)
{
	int gate_entry_idx = -1;

    for (int row = 0; row < GetRows(); ++row)
    {
        for (int column = 0; column < GetColumns(); ++column)
        {
			char c = levelData[row][column];
            auto pNode = GetNode(row, column);
			const auto& pNodeIdx = pNode->GetIndex();
            switch (c)
            {
            case PacData::PacSpawn:
                m_NodeInfoMap[pNodeIdx].type = PacData::PacNodeType::PAC_SPAWN;
				m_PacManSpawnNodeIdx = pNodeIdx;
				break;
			case PacData::Dot:
                m_NodeInfoMap[pNodeIdx].type = PacData::PacNodeType::DOT;
				m_NodeInfoMap[pNodeIdx].hasItem = true;
				++m_totalDots;
				break;
			case PacData::Wall:
                m_NodeInfoMap[pNodeIdx].type = PacData::PacNodeType::WALL;
				break;
			case PacData::Empty:
                m_NodeInfoMap[pNodeIdx].type = PacData::PacNodeType::EMPTY;
				break;
			case PacData::PowerUp:
                m_NodeInfoMap[pNodeIdx].type = PacData::PacNodeType::POWERUP;
				m_NodeInfoMap[pNodeIdx].hasItem = true;
				++m_totalPowerups;
				break;
			case PacData::Gate:
                m_NodeInfoMap[pNodeIdx].type = PacData::PacNodeType::GATE;
				if(gate_entry_idx == -1)
					gate_entry_idx = pNodeIdx;
				else
					AddConnection(new GraphConnection(gate_entry_idx, pNodeIdx, 1));
				break;
			default:
				if (std::isdigit(c))
				{
					m_NodeInfoMap[pNodeIdx].type = PacData::PacNodeType::NPC_SPAWN;
					m_NPCSpawnNodeIdxs.emplace_back(pNodeIdx);
				}
				break;
			}
		}
	}


}

void dae::PacGrid::UpdateConnections()
{
	auto& connectionsList = GetAllConnections();

	for (auto& connections : connectionsList)
	{
		for (auto& connection : connections)
		{
			int toIdx = connection->GetTo();
			int fromIdx = connection->GetFrom();

			auto& toNodeInfo = m_NodeInfoMap[toIdx];
			auto& fromNodeInfo = m_NodeInfoMap[fromIdx];

			if (toNodeInfo.type == PacData::PacNodeType::WALL
				|| fromNodeInfo.type == PacData::PacNodeType::WALL)
			{
				connection->SetCost(FLT_MAX);
			}
			else if (toNodeInfo.type == PacData::PacNodeType::EMPTY && fromNodeInfo.type == PacData::PacNodeType::NPC_SPAWN)
			{
				connection->SetCost(0);
			}
			else if (toNodeInfo.type == PacData::PacNodeType::NPC_SPAWN && fromNodeInfo.type == PacData::PacNodeType::EMPTY)
			{
				connection->SetCost(1000);
			}
			else
			{
				connection->SetCost(-1000);
			}
		}
	}
}
 