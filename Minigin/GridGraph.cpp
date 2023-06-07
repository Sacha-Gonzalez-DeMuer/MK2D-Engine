#include "GridGraph.h"
#include "GraphNode.h"
#include "GraphConnection.h"
#include "MathHelpers.h"

namespace dae
{

	GridGraph::GridGraph(bool isDirectional)
		: IGraph(isDirectional)
		, m_NrOfColumns(0)
		, m_NrOfRows(0)
		, m_CellSize(5)
		, m_IsConnectedDiagonally(true)
		, m_DefaultCostStraight(1.f)
		, m_DefaultCostDiagonal(1.5f)
	{
	}

	GridGraph::GridGraph(
		int columns,
		int rows,
		int cellSize,
		bool isDirectionalGraph,
		bool isConnectedDiagonally,
		float costStraight /* = 1.f*/,
		float costDiagonal /* = 1.5f */)
		: IGraph(isDirectionalGraph)
		, m_NrOfColumns(columns)
		, m_NrOfRows(rows)
		, m_CellSize(cellSize)
		, m_IsConnectedDiagonally(isConnectedDiagonally)
		, m_DefaultCostStraight(costStraight)
		, m_DefaultCostDiagonal(costDiagonal)
	{
		InitializeGrid(columns, rows, cellSize, isDirectionalGraph, isConnectedDiagonally, costStraight, costDiagonal);
	}


	void GridGraph::InitializeGrid(
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
				GraphNode* n = new GraphNode(idx, { (r * m_CellSize), (c * m_CellSize) });
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

	GraphNode* GridGraph::GetNode(int col, int row) const
	{
		return this->m_Nodes[GetIndex(col, row)];
	}

	const GridGraph::ConnectionList& GridGraph::GetConnections(GraphNode* node) const
	{
		return this->m_Connections[node->GetIndex()];
	}

	const GridGraph::ConnectionList& GridGraph::GetConnections(int idx) const
	{
		{ return this->m_Connections[idx]; }
	}


	int GridGraph::GetIndexUp(int currentIndex)
	{
		int up = currentIndex - 1;
		if (up < 0 || up > this->m_Nodes.size() || ((up + 1) % m_NrOfColumns) == 0)
			return -1;

		return up;
	}


	int GridGraph::GetIndexRight(int currentIndex)
	{
		int right = currentIndex + m_NrOfColumns;
		if (right < 0 || right > this->m_Nodes.size() || (currentIndex % m_NrOfColumns) == 0)
			return -1;

		return right;
	}

	int GridGraph::GetIndexDown(int currentIndex)
	{
		int down = currentIndex + 1;
		if (down < 0 || down > this->m_Nodes.size() || (down % m_NrOfColumns) == 0)
			return -1;

		return down;
	}

	int GridGraph::GetIndexLeft(int currentIndex)
	{
		int left = currentIndex - m_NrOfColumns;
		if (left < 0 || left > this->m_Nodes.size() || (left % m_NrOfColumns) == 0)
			return -1;
		return left;
	}


	bool GridGraph::IsWithinBounds(int col, int row) const
	{
		return (col >= 0 && col < m_NrOfColumns && row >= 0 && row < m_NrOfRows);
	}



	void GridGraph::AddConnectionsToAdjacentCells(int col, int row)
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


	void GridGraph::AddConnectionsToAdjacentCells(int idx)
	{
		auto colRow = IGraph::GetNodePos(idx);
		AddConnectionsToAdjacentCells((int)colRow.x, (int)colRow.y);
	}

	Direction GridGraph::GetConnectionDirection(int fromIdx, int toIdx) const
	{
		int fromRow = fromIdx % m_NrOfRows;    // Calculate row index from graph index
		int fromCol = fromIdx / m_NrOfRows;    // Calculate column index from graph index

		int toRow = toIdx % m_NrOfRows;        // Calculate row index from graph index
		int toCol = toIdx / m_NrOfRows;        // Calculate column index from graph index

		if (fromCol == toCol && fromRow + 1 == toRow)
			return Direction::DOWN;
		if (fromCol == toCol && fromRow - 1 == toRow)
			return Direction::UP;
		if (fromCol + 1 == toCol && fromRow == toRow)
			return Direction::RIGHT;
		if (fromCol - 1 == toCol && fromRow == toRow)
			return Direction::LEFT;

		return Direction::NONE;
	}

	void GridGraph::AddConnectionsInDirections(int idx, int col, int row, std::vector<glm::vec2> directions)
	{
		for (const auto& d : directions)
		{
			int neighborCol = col + (int)d.x;
			int neighborRow = row + (int)d.y;

			if (IsWithinBounds(neighborCol, neighborRow))
			{
				int neighborIdx = neighborRow * m_NrOfColumns + neighborCol;
				float connectionCost = CalculateConnectionCost(idx, neighborIdx);

				if (this->IsUniqueConnection(idx, neighborIdx)
					&& connectionCost < 100000) //Extra check for different terrain types
					this->AddConnection(new GraphConnection(idx, neighborIdx, connectionCost));
			}
		}
	}


	float GridGraph::CalculateConnectionCost(int fromIdx, int toIdx) const
	{
		float cost = m_DefaultCostStraight;

		glm::vec2 fromPos = IGraph::GetNodePos(fromIdx);
		glm::vec2 toPos = IGraph::GetNodePos(toIdx);

		if (int(fromPos.y) != int(toPos.y) &&
			int(fromPos.x) != int(toPos.x))
		{
			cost = m_DefaultCostDiagonal;
		}

		return cost;
	}


	glm::vec2 GridGraph::GetNodePos(GraphNode* pNode) const
	{
		auto col = pNode->GetIndex() % m_NrOfColumns;
		auto row = pNode->GetIndex() / m_NrOfColumns;


		return glm::vec2{ float(col), float(row) };
	}


	glm::vec2 GridGraph::GetNodeWorldPos(int col, int row) const
	{
		glm::vec2 cellCenterOffset = { m_CellSize / 2.f, m_CellSize / 2.f };
		return glm::vec2{ (float)col * m_CellSize, (float)row * m_CellSize } + cellCenterOffset;
	}



	int GridGraph::GetNodeIdxAtWorldPos(const glm::vec2& pos) const
	{
		int idx = invalid_node_index;
		float halfCellSize = static_cast<float>(m_CellSize) * .5f;
		glm::vec2 position{ pos };
		position.x -= m_Offset.x - halfCellSize;
		position.y -= m_Offset.y - halfCellSize;
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



	void GridGraph::GetNodesInRadius(std::unordered_set<int>& idxCache, float radius, const glm::vec2& center) const
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
			if (idxCache.count(node) == 0 && MathHelpers::glmDistanceSquared(IGraph::GetNode(node)->GetPosition(), center) <= radius * radius)
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

}