#include "PacNavigator.h"

dae::PacNavigator::PacNavigator(std::shared_ptr<GridGraph> graph, std::shared_ptr<IPathFinder> pathfinder)
	: GridNavComponent(graph, pathfinder)
{
}

void dae::PacNavigator::Update()
{
	if (SqrDistanceToTarget() < 0.1f)
	{
		Move(m_CurrentDirection);
	}
}
