#pragma once
#include "IPathFinder.h"
#include <vector>
#include <memory>
namespace dae
{
	class IGraph;
	class GraphNode;
	class GraphConnection;
	class AStarPathFinder : public IPathFinder
	{
	public:
		AStarPathFinder(std::shared_ptr<IGraph> graph);
		~AStarPathFinder() = default;
		std::vector<GraphNode*> FindPath(GraphNode* pStartNode, GraphNode* pDestinationNode) override;

		// stores the optimal connection to a node and its total costs related to the start and end node of the path
		struct NodeRecord
		{
			GraphNode* pNode = nullptr;
			GraphConnection* pConnection = nullptr;
			float costSoFar = 0.f; // accumulated g-costs of all the connections leading up to this one
			float estimatedTotalCost = 0.f; // f-cost (= costSoFar + h-cost)

			bool operator==(const NodeRecord& other) const
			{
				return pNode == other.pNode
					&& pConnection == other.pConnection
					&& costSoFar == other.costSoFar
					&& estimatedTotalCost == other.estimatedTotalCost;
			};

			bool operator<(const NodeRecord& other) const
			{
				return estimatedTotalCost < other.estimatedTotalCost;
			};
		};

	private:
		float GetHeuristicCost(GraphNode* pStartNode, GraphNode* pEndNode) const;

		std::shared_ptr<IGraph> m_pGraph;

		NodeRecord GetLowestFScoreConnection(std::vector<NodeRecord> fromList) const;
	};

}