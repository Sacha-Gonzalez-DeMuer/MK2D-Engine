#include "PacGrid.h"
#include "PacData.h"
#include "GraphNode.h"
#include "GraphConnection.h"
dae::PacGrid::PacGrid(const std::vector<std::string>& levelData)
	: GridGraph(static_cast<int>(levelData.size()), static_cast<int>(levelData[0].size()), 50, false, false)
	, m_NodeInfoMap(21*21)
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

const glm::vec2& dae::PacGrid::GetSpawnPos() const
{
	return GetNode(m_PacManSpawnNodeIdx)->GetPosition();
}

void dae::PacGrid::Initialize(const std::vector<std::string>& levelData)
{
    for (int row = 0; row < GetRows(); ++row)
    {
        for (int column = 0; column < GetColumns(); ++column)
        {
			char c = levelData[row][column];
            auto pNode = GetNode(row, column);
            switch (c)
            {
            case PacData::PacMan:
                m_NodeInfoMap[pNode->GetIndex()].type = PacData::PacNodeType::SPAWN;
				m_PacManSpawnNodeIdx = pNode->GetIndex();
				break;
			case PacData::Dot:
                m_NodeInfoMap[pNode->GetIndex()].type = PacData::PacNodeType::DOT;
				break;
			case PacData::Wall:
                m_NodeInfoMap[pNode->GetIndex()].type = PacData::PacNodeType::WALL;
				break;
			case PacData::Empty:
                m_NodeInfoMap[pNode->GetIndex()].type = PacData::PacNodeType::EMPTY;
				break;
			case PacData::PowerUp:
                m_NodeInfoMap[pNode->GetIndex()].type = PacData::PacNodeType::POWERUP;
				break;
			case PacData::Gate:
                m_NodeInfoMap[pNode->GetIndex()].type = PacData::PacNodeType::GATE;
				break;
			default:
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
				connection->SetCost(1000);
			}
			else
			{
				connection->SetCost(1);
			}
		}
	}
}
 