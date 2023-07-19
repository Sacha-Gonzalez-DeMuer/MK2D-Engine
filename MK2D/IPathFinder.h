#pragma once
#include <vector>
#include "GraphNode.h"


namespace dae
{
	class IPathFinder
	{
	public:
		virtual ~IPathFinder() = default;
		virtual std::vector<GraphNode*> FindPath(GraphNode* pStartNode, GraphNode* pDestinationNode) = 0;
	};
}


