#include "CGraphNodeNavigator.h"
#include "IGraph.h"
#include "Graph2D.h"
#include "glm/glm.hpp"

dae::Graph2DNavComponent::Graph2DNavComponent(std::shared_ptr<Graph2D> graph, std::shared_ptr<GraphNode2D> startingNode)
	: m_pGraph(graph)
	, m_CurrentNode(startingNode)
	, m_TargetNode(startingNode)
{
}

void dae::Graph2DNavComponent::Move(const Direction& direction)
{
	const auto& connections{ m_pGraph->GetNodeConnections(m_CurrentNode.get()) };

	glm::vec2 connectionDir;
	for (const auto& connection : connections)
	{
		// it is at this moment, he knew, he...
		// i should use a grid to easily get the neighbor in a given direction
	}
}

void dae::Graph2DNavComponent::GoTo(std::shared_ptr<GraphNode2D> /*node*/)
{
}

void dae::Graph2DNavComponent::Update()
{
}
