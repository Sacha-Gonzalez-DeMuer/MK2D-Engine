#include "CGraphNodeNavigator.h"
#include "glm/glm.hpp"
#include "AStarPathFinder.h"

#include "GraphNode.h"
#include "GraphConnection.h"
#include "GridGraph.h"
#include "IPathFinder.h"

#include <memory>
#include <iostream>

namespace dae
{
	GridNavComponent::GridNavComponent(std::shared_ptr<GridGraph> graph, 
		std::shared_ptr<IPathFinder> pathfinder)
		: m_pGraph(graph)
		, m_CurrentNode(m_pGraph->GetNode(1, 1))
		, m_TargetNode(m_pGraph->GetNode(1, 1))
		, m_pPathFinder(std::move(pathfinder))
	{
	}

	void dae::GridNavComponent::Move(Direction direction)
	{
		int to_node_idx = -1;
		GraphNode* to_node;
		std::cout << "moved from node: " << m_CurrentNode->GetIndex();

		//print direction
		std::cout << " in direction: " << int(direction) <<" \n";

		switch (direction)
		{
		case Direction::UP:
			to_node_idx = m_pGraph->GetIndexUp(m_CurrentNode->GetIndex());
			break;
		case Direction::RIGHT:
			to_node_idx = m_pGraph->GetIndexRight(m_CurrentNode->GetIndex());
			break;
		case Direction::DOWN:
			to_node_idx = m_pGraph->GetIndexDown(m_CurrentNode->GetIndex());
			break;
		case Direction::LEFT:
			to_node_idx = m_pGraph->GetIndexLeft(m_CurrentNode->GetIndex());
			break;
		default:
			break;
		}

		if (to_node_idx != -1)
		{
			to_node = m_pGraph->GetNode(to_node_idx);
			m_CurrentNode = to_node;
			m_gameObject.lock()->GetTransform()->SetLocalPosition(m_CurrentNode->GetPosition());
			std::cout << "to: " << m_CurrentNode->GetIndex() << "\n";
		} 
		else
		{
			std::cout << "to: " << "[out of bounds]" << "\n";
		}
	}
	void GridNavComponent::GoTo(std::shared_ptr<GraphNode> node)
	{
	}
	void GridNavComponent::Start()
	{
	}
	void GridNavComponent::Update()
	{
	}
}
