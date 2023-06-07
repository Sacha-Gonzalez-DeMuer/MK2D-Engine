#include "PacGrid.h"
#include "PacData.h"
#include "GraphNode.h"

dae::PacGrid::PacGrid(const std::vector<std::string>& levelData)
	: GridGraph(static_cast<int>(levelData.size()), static_cast<int>(levelData[0].size()), 50, false, false)
	, m_NodeInfoMap(21*21)
{
	Initialize(levelData);
}

dae::PacGridData::PacNodeInfo dae::PacGrid::GetPacNodeInfo(int nodeIdx) const
{
	return m_NodeInfoMap[nodeIdx];
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
            case dae::LevelData::PacMan:
                m_NodeInfoMap[pNode->GetIndex()].type = PacGridData::PacNodeType::Spawn;
				m_PacManSpawnNodeIdx = pNode->GetIndex();
				break;
			case dae::LevelData::Dot:
                m_NodeInfoMap[pNode->GetIndex()].type = PacGridData::PacNodeType::Dot;
				break;
			case dae::LevelData::Wall:
                m_NodeInfoMap[pNode->GetIndex()].type = PacGridData::PacNodeType::Wall;
				break;
			case dae::LevelData::Empty:
                m_NodeInfoMap[pNode->GetIndex()].type = PacGridData::PacNodeType::Empty;
				break;
			case dae::LevelData::PowerUp:
                m_NodeInfoMap[pNode->GetIndex()].type = PacGridData::PacNodeType::PowerUp;
				break;
			case dae::LevelData::Gate:
                m_NodeInfoMap[pNode->GetIndex()].type = PacGridData::PacNodeType::Gate;
				break;
			default:
				break;
			}
		}
	}


}
 