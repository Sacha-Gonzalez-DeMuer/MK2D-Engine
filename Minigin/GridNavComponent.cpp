#include "GridNavComponent.h"
#include "GridNavComponent.h"
#include "glm/glm.hpp"
#include "AStarPathFinder.h"
#include "GraphNode.h"
#include "GraphConnection.h"
#include "GridGraph.h"
#include "IPathFinder.h"
#include "MathHelpers.h"
#include "GameTime.h"

#include <memory>
#include <iostream>

namespace dae
{
	GridNavComponent::GridNavComponent(std::shared_ptr<GridGraph> graph,
		std::shared_ptr<IPathFinder> pathfinder)
		: m_pGraph(graph)
		, m_CurrentNode(graph->GetNode(0))
		, m_TargetNode(graph->GetNode(0))
		, m_pPathFinder(pathfinder)
	{
	}

	void GridNavComponent::Start()
	{
		m_gameObject.lock()->SetLocalPosition(m_CurrentNode->GetPosition());
	}

	void dae::GridNavComponent::Move(Direction direction)
	{
		float distance_to_enable_queuing{ 20.0f }; // distance at which a move will be queued again
		float dist_to_target = MathHelpers::glmDistanceSquared(m_gameObject.lock()->GetWorldPosition(), m_TargetNode->GetPosition());

		if (m_Path.size() >= 1 && dist_to_target > distance_to_enable_queuing) return;
		else if (m_Path.size() >= 2) return;

		int to_node_idx = -1;
		GraphNode* to_node;

		int from_node_idx = m_Path.empty() ? m_CurrentNode->GetIndex() : m_TargetNode->GetIndex();
		switch (direction)
		{
		case Direction::UP:
			to_node_idx = m_pGraph->GetIndexUp(from_node_idx);
			break;
		case Direction::RIGHT:
			to_node_idx = m_pGraph->GetIndexRight(from_node_idx);
			break;
		case Direction::DOWN:
			to_node_idx = m_pGraph->GetIndexDown(from_node_idx);
			break;
		case Direction::LEFT:
			to_node_idx = m_pGraph->GetIndexLeft(from_node_idx);
			break;
		default:
			break;
		}

		if (to_node_idx != -1)
		{
			to_node = m_pGraph->GetNode(to_node_idx);
			AddMoveToPath(to_node);
		}
	}

	void GridNavComponent::GoTo(GraphNode* node)
	{
		m_TargetNode = node;
		auto path = m_pPathFinder->FindPath(m_CurrentNode, m_TargetNode);

		for (const auto& path_node : path)
		{
			AddMoveToPath(path_node);
		}
	}

	float GridNavComponent::SqrDistanceToTarget() const
	{
		return MathHelpers::glmDistanceSquared(m_gameObject.lock()->GetWorldPosition(), m_TargetNode->GetPosition());
	}

	void GridNavComponent::Update()
	{
		constexpr float epsilon = 1.0f;
		if (!m_Path.empty())
		{
			m_TargetNode = m_Path.front();
			float distance_to_target = MathHelpers::glmDistanceSquared(m_gameObject.lock()->GetWorldPosition(), m_TargetNode->GetPosition());

			// If we're close enough to the target node, snap to it and move on to the next one
			if (abs(distance_to_target) <= epsilon)
			{
				m_gameObject.lock()->GetTransform()->SetLocalPosition(m_TargetNode->GetPosition());
				m_CurrentNode = m_TargetNode;
				m_Path.pop(); // Remove the current node from the path

				// If we've reached the end of the path, stop moving
				if (m_Path.empty())
					return;
				else
				{
					// Otherwise, set the target node to the next node in the path
					m_TargetNode = m_Path.front();
					m_Path.pop();
				}

			}
			else
			{
				// Otherwise, interpolate towards the target node
				const auto& current_position = m_gameObject.lock()->GetTransform()->GetLocalPosition();
				const auto& direction = glm::normalize(m_TargetNode->GetPosition() - current_position);
				const float speed = m_MoveSpeed * GameTime::GetInstance().DeltaTime();
				auto new_position = current_position + direction * speed;

				// If we've overshot the target node, snap to it
				if (MathHelpers::glmDistanceSquared(new_position, m_TargetNode->GetPosition()) > distance_to_target)
					m_gameObject.lock()->GetTransform()->SetLocalPosition(m_TargetNode->GetPosition());


				m_gameObject.lock()->GetTransform()->SetLocalPosition(new_position);
			}
		}
	}

	void GridNavComponent::AddMoveToPath(GraphNode* toNode)
	{
		if (m_Path.empty() || m_Path.front() != toNode)
		{
			std::cout << "Adding move!\n";
			m_Path.push(toNode);
		}
	}
}