#include "CGraphNodeNavigator.h"

dae::Graph2DNavComponent::Graph2DNavComponent(std::shared_ptr<Graph2D> graph, std::shared_ptr<GraphNode2D> node)
	: m_pGraph(graph)
	, m_CurrentNode(node)
	, m_TargetNode(node)
{
}

void dae::Graph2DNavComponent::Move(const Direction& direction)
{
}

void dae::Graph2DNavComponent::GoTo(std::shared_ptr<GraphNode2D> node)
{
}

void dae::Graph2DNavComponent::Update()
{
}
