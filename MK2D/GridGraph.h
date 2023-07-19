#pragma once
#include "IGraph.h"
#include <glm/glm.hpp>
#include <queue>
#include "Datatypes.h"

namespace dae
{
	class GridGraph : public IGraph
	{
	public:
		using NodeVector = std::vector<GraphNode*>;
		using ConnectionList = std::list<GraphConnection*>;
		using ConnectionListVector = std::vector<ConnectionList>;
		const int invalid_node_index = -1;

	public:
		GridGraph(bool isDirectional);
		GridGraph(int columns, int rows, int cellSize, bool isDirectionalGraph, bool isConnectedDiagonally, float costStraight = 1.f, float costDiagonal = 1.5);
		void InitializeGrid(int columns, int rows, int cellSize, bool isDirectionalGraph, bool isConnectedDiagonally, float costStraight = 1.f, float costDiagonal = 1.5);

		using IGraph::GetNode;
		GraphNode* GetNode(int col, int row) const;
		const ConnectionList& GetConnections(GraphNode* node) const;
		const ConnectionList& GetConnections(int idx) const;

		int GetRows() const { return m_NrOfRows; }
		int GetColumns() const { return m_NrOfColumns; }
		int GetCellSize() const { return m_CellSize; }

		int GetIndexUp(int currentIndex);
		int GetIndexRight(int currentIndex);
		int GetIndexDown(int currentIndex);
		int GetIndexLeft(int currentIndex);
			
		bool IsWithinBounds(int col, int row) const;
		int GetIndex(int col, int row) const { return row * m_NrOfColumns + col; }

		// returns the column and row of the node in a glm::vec2
		virtual glm::vec2 GetNodePos(GraphNode* pNode) const override;

		// returns the actual world position of the node
		glm::vec2 GetNodeWorldPos(int col, int row) const;

		int GetNodeIdxAtWorldPos(const glm::vec2& pos) const override;
		void AddConnectionsToAdjacentCells(int col, int row);
		void AddConnectionsToAdjacentCells(int idx);
		GridGraph* GetGraph() { return this; };

		Direction GetConnectionDirection(int fromIdx, int toIdx) const;

	private:
		int m_NrOfColumns;
		int m_NrOfRows;
		int m_CellSize;
		glm::vec2 m_Offset{};

		bool m_IsConnectedDiagonally;
		float m_DefaultCostStraight;
		float m_DefaultCostDiagonal;

		const std::vector<glm::vec2> m_StraightDirections = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
		const std::vector<glm::vec2> m_DiagonalDirections = { { 1, 1 }, { -1, 1 }, { -1, -1 }, { 1, -1 } };

		// graph creation helper functions
		void AddConnectionsInDirections(int idx, int col, int row, std::vector<glm::vec2> directions);

		float CalculateConnectionCost(int fromIdx, int toIdx) const;

		void GetNodesInRadius(std::unordered_set<int>& idxCache, float radius, const glm::vec2& center) const;
		friend class GraphRenderer;
	};

	
}

