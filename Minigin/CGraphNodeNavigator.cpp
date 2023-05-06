#include "CGraphNodeNavigator.h"
#include "GridGraph.h"
#include "glm/glm.hpp"
#include <memory>
#include "AStarPathFinder.h"
#include "GraphConnectionTypes.h"
#include <iostream>

namespace dae
{
	GridNavComponent::GridNavComponent(std::shared_ptr<GridGraph<GraphNode2D, GraphConnection2D>> graph, 
		std::shared_ptr<IPathFinder<GraphNode2D, GraphConnection2D>> pathfinder)
		: m_pGraph(graph)
		, m_CurrentNode(m_pGraph->GetNode(10, 10))
		, m_pPathFinder(std::move(pathfinder))
	{
	}

	void dae::GridNavComponent::Move(Direction direction)
	{
		int to_node_idx = -1;
		GraphNode2D* to_node;
		std::cout << "moved from node: " << m_CurrentNode->GetIndex();

		switch (direction)
		{
		case Direction::UP:
			to_node_idx = m_pGraph->GetIndexUp(m_CurrentNode->GetIndex());

		case Direction::RIGHT:
			to_node_idx = m_pGraph->GetIndexRight(m_CurrentNode->GetIndex());

		case Direction::DOWN:
			to_node_idx = m_pGraph->GetIndexDown(m_CurrentNode->GetIndex());

		case Direction::LEFT:
			to_node_idx = m_pGraph->GetIndexLeft(m_CurrentNode->GetIndex());

		default:
			break;
		}

		to_node = m_pGraph->GetNode(to_node_idx);
		std::cout << "to: " << m_CurrentNode->GetIndex() << "\n";

	}

	void dae::GridNavComponent::GoTo(std::shared_ptr<GraphNode2D> /*node*/)
	{
	}

	void dae::GridNavComponent::Update()
	{
	}

}
