#pragma once
#include "GridGraph.h"
#include <string>
#include <unordered_map>
#include "PacData.h"
#include <vector>

namespace dae
{
	// struct that holds info about a node
	class PacGrid final : public GridGraph
	{
	public:
		PacGrid(const std::vector<std::string>& levelData);
		~PacGrid() = default;

		PacGridData::PacNodeInfo GetPacNodeInfo(int nodeIdx) const;
		int GetSpawnIdx() const { return m_PacManSpawnNodeIdx; }
		const glm::vec2& GetSpawnPos() const;

	private:
		void Initialize(const std::vector<std::string>& levelData);
		void UpdateConnections();

		int m_PacManSpawnNodeIdx{};
		std::vector<PacGridData::PacNodeInfo> m_NodeInfoMap{};
	};
}


