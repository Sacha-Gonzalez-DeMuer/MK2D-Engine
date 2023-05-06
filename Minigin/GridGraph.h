#pragma once
#include "IGraph.h"
#include <glm/glm.hpp>
#include <queue>
#include "MathHelpers.h"

namespace dae
{
	template<class T_NodeType, class T_ConnectionType>
	class GridGraph : public IGraph<T_NodeType, T_ConnectionType>
	{
	public:
		using NodeVector = std::vector<T_NodeType*>;
		using ConnectionList = std::list<T_ConnectionType*>;
		using ConnectionListVector = std::vector<ConnectionList>;
		const int invalid_node_index = -1;

	public:
		GridGraph(bool isDirectional);
		GridGraph(int columns, int rows, int cellSize, bool isDirectionalGraph, bool isConnectedDiagonally, float costStraight = 1.f, float costDiagonal = 1.5);
		void InitializeGrid(int columns, int rows, int cellSize, bool isDirectionalGraph, bool isConnectedDiagonally, float costStraight = 1.f, float costDiagonal = 1.5);

		using IGraph<T_NodeType, T_ConnectionType>::GetNode;
		T_NodeType* GetNode(int col, int row) const { return this->m_Nodes[GetIndex(col, row)]; }
		const ConnectionList& GetConnections(const T_NodeType& node) const { return this->m_Connections[node.GetIndex()]; }
		const ConnectionList& GetConnections(int idx) const { return this->m_Connections[idx]; }

		int GetRows() const { return m_NrOfRows; }
		int GetColumns() const { return m_NrOfColumns; }
		int GetCellSize() const { return m_CellSize; }

		int GetIndexUp(int currentIndex) { return currentIndex - m_NrOfColumns; }
		int GetIndexRight(int currentIndex) { return currentIndex + 1; }
		int GetIndexDown(int currentIndex){ return currentIndex + m_NrOfColumns; }
		int GetIndexLeft(int currentIndex) { return currentIndex - 1; }

		bool IsWithinBounds(int col, int row) const;
		int GetIndex(int col, int row) const { return row * m_NrOfColumns + col; }

		// returns the column and row of the node in a glm::vec2
		virtual glm::vec2 GetNodePos(T_NodeType* pNode) const override;

		// returns the actual world position of the node
		glm::vec2 GetNodeWorldPos(int col, int row) const;

		int GetNodeIdxAtWorldPos(const glm::vec2& pos) const override;
		void AddConnectionsToAdjacentCells(int col, int row);
		void AddConnectionsToAdjacentCells(int idx);
		GridGraph* GetGraph() { return this; };

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

		void GetNodesInRadiusRecursive(T_NodeType* node, std::unordered_set<int>& idxCache, float radius, const glm::vec2& center) const;
		void GetNodesInSquareRecursive(T_NodeType* node, std::unordered_set<int>& idxCache, const glm::vec2& position, const glm::vec2& size) const;
		void GetNodesInRadius(std::unordered_set<int>& idxCache, float radius, const glm::vec2& center) const;
		friend class GraphRenderer;
	};

	template<class T_NodeType, class T_ConnectionType>
	inline GridGraph<T_NodeType, T_ConnectionType>::GridGraph(bool isDirectional)
		: IGraph<T_NodeType, T_ConnectionType>(isDirectional)
		, m_NrOfColumns(0)
		, m_NrOfRows(0)
		, m_CellSize(5)
		, m_IsConnectedDiagonally(true)
		, m_DefaultCostStraight(1.f)
		, m_DefaultCostDiagonal(1.5f)
	{
	}

	template<class T_NodeType, class T_ConnectionType>
	GridGraph<T_NodeType, T_ConnectionType>::GridGraph(
		int columns,
		int rows,
		int cellSize,
		bool isDirectionalGraph,
		bool isConnectedDiagonally,
		float costStraight /* = 1.f*/,
		float costDiagonal /* = 1.5f */)
		: IGraph<T_NodeType, T_ConnectionType>(isDirectionalGraph)
		, m_NrOfColumns(columns)
		, m_NrOfRows(rows)
		, m_CellSize(cellSize)
		, m_IsConnectedDiagonally(isConnectedDiagonally)
		, m_DefaultCostStraight(costStraight)
		, m_DefaultCostDiagonal(costDiagonal)
	{
		InitializeGrid(columns, rows, cellSize, isDirectionalGraph, isConnectedDiagonally, costStraight, costDiagonal);
	}

	template<class T_NodeType, class T_ConnectionType>
	inline void GridGraph<T_NodeType, T_ConnectionType>::InitializeGrid(
		int columns,
		int rows,
		int cellSize,
		bool isDirectionalGraph,
		bool isConnectedDiagonally,
		float costStraight /* = 1.f*/,
		float costDiagonal /* = 1.5f */)
	{
		this->m_IsDirectionalGraph = isDirectionalGraph;
		m_NrOfColumns = columns;
		m_NrOfRows = rows;
		m_CellSize = cellSize;
		m_IsConnectedDiagonally = isConnectedDiagonally;
		m_DefaultCostStraight = costStraight;
		m_DefaultCostDiagonal = costDiagonal;

		// Create all nodes
		for (auto r = 0; r < m_NrOfRows; ++r)
		{
			for (auto c = 0; c < m_NrOfColumns; ++c)
			{
				int idx = GetIndex(c, r);
				T_NodeType* n = new T_NodeType(idx, { (r * m_CellSize), (c * m_CellSize) });
				this->AddNode(n);
			}
		}

		// Create connections in each valid direction on each node
		for (auto r = 0; r < m_NrOfRows; ++r)
		{
			for (auto c = 0; c < m_NrOfColumns; ++c)
			{
				AddConnectionsToAdjacentCells(c, r);
			}
		}
	}

	template<class T_NodeType, class T_ConnectionType>
	bool GridGraph<T_NodeType, T_ConnectionType>::IsWithinBounds(int col, int row) const
	{
		return (col >= 0 && col < m_NrOfColumns && row >= 0 && row < m_NrOfRows);
	}


	template<class T_NodeType, class T_ConnectionType>
	void GridGraph<T_NodeType, T_ConnectionType>::AddConnectionsToAdjacentCells(int col, int row)
	{
		int idx = GetIndex(col, row);

		// Add connections in all directions, taking into account the dimensions of the grid
		AddConnectionsInDirections(idx, col, row, m_StraightDirections);

		if (m_IsConnectedDiagonally)
		{
			AddConnectionsInDirections(idx, col, row, m_DiagonalDirections);
		}

		this->OnGraphModified(false, true);
	}

	template<class T_NodeType, class T_ConnectionType>
	inline void GridGraph<T_NodeType, T_ConnectionType>::AddConnectionsToAdjacentCells(int idx)
	{
		auto colRow = IGraph<T_NodeType, T_ConnectionType>::GetNodePos(idx);
		AddConnectionsToAdjacentCells((int)colRow.x, (int)colRow.y);
	}

	template<class T_NodeType, class T_ConnectionType>
	void GridGraph<T_NodeType, T_ConnectionType>::AddConnectionsInDirections(int idx, int col, int row, std::vector<glm::vec2> directions)
	{
		for (auto d : directions)
		{
			int neighborCol = col + (int)d.x;
			int neighborRow = row + (int)d.y;

			if (IsWithinBounds(neighborCol, neighborRow))
			{
				int neighborIdx = neighborRow * m_NrOfColumns + neighborCol;
				float connectionCost = CalculateConnectionCost(idx, neighborIdx);

				if (this->IsUniqueConnection(idx, neighborIdx)
					&& connectionCost < 100000) //Extra check for different terrain types
					this->AddConnection(new T_ConnectionType(idx, neighborIdx, connectionCost));
			}
		}
	}

	template<class T_NodeType, class T_ConnectionType>
	inline float GridGraph<T_NodeType, T_ConnectionType>::CalculateConnectionCost(int fromIdx, int toIdx) const
	{
		float cost = m_DefaultCostStraight;

		glm::vec2 fromPos = IGraph<T_NodeType, T_ConnectionType>::GetNodePos(fromIdx);
		glm::vec2 toPos = IGraph<T_NodeType, T_ConnectionType>::GetNodePos(toIdx);

		if (int(fromPos.y) != int(toPos.y) &&
			int(fromPos.x) != int(toPos.x))
		{
			cost = m_DefaultCostDiagonal;
		}

		return cost;
	}

	template<class T_NodeType, class T_ConnectionType>
	glm::vec2 GridGraph<T_NodeType, T_ConnectionType>::GetNodePos(T_NodeType* pNode) const
	{
		auto col = pNode->GetIndex() % m_NrOfColumns;
		auto row = pNode->GetIndex() / m_NrOfColumns;


		return glm::vec2{ float(col), float(row) };
	}

	template<class T_NodeType, class T_ConnectionType>
	glm::vec2 GridGraph<T_NodeType, T_ConnectionType>::GetNodeWorldPos(int col, int row) const
	{
		glm::vec2 cellCenterOffset = { m_CellSize / 2.f, m_CellSize / 2.f };
		return glm::vec2{ (float)col * m_CellSize, (float)row * m_CellSize } + cellCenterOffset;
	}


	template<class T_NodeType, class T_ConnectionType>
	inline int GridGraph<T_NodeType, T_ConnectionType>::GetNodeIdxAtWorldPos(const glm::vec2& pos) const
	{
		int idx = invalid_node_index;

		glm::vec2 position{ pos };
		position.x -= m_Offset.x - m_CellSize / 2;
		position.y -= m_Offset.y - m_CellSize / 2;
		//Added extra check since  c = int(pos.x / m_CellSize); => doesnt work correcly when out of the lower bounds
		//TODO add grid start point
		if (position.x < 0 || position.y < 0)
		{
			return idx;
		}

		int r, c;

		r = static_cast<int>(position.x / m_CellSize);
		c = static_cast<int>(position.y / m_CellSize);

		if (!IsWithinBounds(c, r))
			return idx;

		return GetIndex(c, r);
	}



	template<class T_NodeType, class T_ConnectionType>
	void GridGraph<T_NodeType, T_ConnectionType>::GetNodesInRadiusRecursive(T_NodeType* node, std::unordered_set<int>& idxCache, float radius, const glm::vec2& center) const
	{
		// Check if the current node is within the specified radius of the center point
		if (MathHelpers::glmDistanceSquared(node->GetPosition(), center) > radius * radius || idxCache.count(node->GetIndex() == 0))
		{
			return;
		}

		// Add the current node to the cache
		idxCache.insert(node->GetIndex());

		// Recursively process the child nodes
		for (const auto& connection : this->GetConnections(node->GetIndex()))
		{
			// Check if the child node has already been processed
			if (idxCache.count(connection->GetTo()) > 0)
			{
				continue;
			}

			auto childNode{ IGraph<T_NodeType, T_ConnectionType>::GetNode(connection->GetTo()) };
			GetNodesInRadiusRecursive(childNode, idxCache, radius, center);
		}
	}



	template<class T_NodeType, class T_ConnectionType>
	void GridGraph<T_NodeType, T_ConnectionType>::GetNodesInRadius(std::unordered_set<int>& idxCache, float radius, const glm::vec2& center) const
	{
		// Create a queue to store the nodes that need to be processed
		std::queue<int> nodesToProcess;

		// Add the root node to the queue
		nodesToProcess.push(GetNodeIdxAtWorldPos(center));

		// Process the nodes in the queue
		while (!nodesToProcess.empty())
		{
			// Get the next node from the queue
			auto node = nodesToProcess.front();
			nodesToProcess.pop();

			// Check if the current node is within the specified radius of the center point
			if (idxCache.count(node) == 0 && MathHelpers::glmDistanceSquared(IGraph<T_NodeType, T_ConnectionType>::GetNode(node)->GetPosition(), center) <= radius * radius)
			{
				// Add the current node to the cache
				idxCache.insert(node);

				// Add the child nodes to the queue
				for (const auto& connection : GetConnections(node))
				{
					// Check if the child node has already been processed
					if (idxCache.count(connection->GetTo()) == 0)
					{
						nodesToProcess.push(connection->GetTo());
					}
				}
			}
		}
	}

	template<class T_NodeType, class T_ConnectionType>
	void GridGraph<T_NodeType, T_ConnectionType>::GetNodesInSquareRecursive(T_NodeType* node, std::unordered_set<int>& idxCache, const glm::vec2& position, const glm::vec2& size) const
	{
		// Calculate the half width and half height of the square
		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;

		// Check if the current node is within the square
		if (node->GetPosition().x >= position.x - halfWidth && node->GetPosition().x <= position.x + halfWidth &&
			node->GetPosition().y >= position.y - halfHeight && node->GetPosition().y <= position.y + halfHeight)
		{
			// Add the index of the current node to the cache
			idxCache.insert(node->GetIndex());
		}
		else // If the current node is not within the square, return without processing its connections
			return;

		// Recursively process the child nodes
		for (const auto& connection : GetConnections(node->GetIndex()))
		{
			// Check if the child node has already been processed
			if (idxCache.count(connection->GetTo()) > 0)
				continue;

			auto childNode{ IGraph<T_NodeType, T_ConnectionType>::GetNode(connection->GetTo() ) };
			GetNodesInSquareRecursive(childNode, idxCache, position, size);
		}
	}
}

