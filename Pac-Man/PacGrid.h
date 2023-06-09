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
		PacGrid(const std::vector<std::string>& map);
		~PacGrid() = default;

		const PacData::PacNodeInfo& GetPacNodeInfo(int nodeIdx) const;
		void SetPacNodeInfo(PacData::PacNodeInfo nodeInfo, int nodeIdx);
		void SetPacNodeInfo(int idx, PacData::PacNodeType type, bool hasItem = false);

		int GetSpawnIdx() const { return m_PacManSpawnNodeIdx; }
		const glm::vec2& GetSpawnPos() const;

	private:
		void Initialize(const std::vector<std::string>& levelData);
		void UpdateConnections();

		int m_PacManSpawnNodeIdx{};
		std::vector<PacData::PacNodeInfo> m_NodeInfoMap{};
	};
}


