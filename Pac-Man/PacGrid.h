#pragma once
#include "GridGraph.h"
#include <string>
#include <unordered_map>
#include "PacData.h"
#include <vector>
#include "Component.h"

namespace dae
{
	// struct that holds info about a node
	class PacGrid final : public GridGraph, public Component
	{
	public:
		PacGrid(const std::vector<std::string>& map);
		~PacGrid() = default;

		const PacData::PacNodeInfo& GetPacNodeInfo(int nodeIdx) const;
		void SetPacNodeInfo(PacData::PacNodeInfo nodeInfo, int nodeIdx);
		void SetPacNodeInfo(int idx, PacData::PacNodeType type, bool hasItem = false);

		int GetSpawnIdx() const { return m_PacManSpawnNodeIdx; }
		const glm::vec2& GetSpawnPos() const;
		
		virtual glm::vec2 GetNodePos(GraphNode* pNode) const override;
		virtual int GetNodeIdxAtWorldPos(const glm::vec2& worldPos) const override;

	private:
		void Initialize(const std::vector<std::string>& levelData);
		void UpdateConnections();

		int m_PacManSpawnNodeIdx{};
		std::vector<PacData::PacNodeInfo> m_NodeInfoMap{};
	};
}


