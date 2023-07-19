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
#include <SDL.h>
namespace dae
{
	GridNavComponent::GridNavComponent(std::shared_ptr<GridGraph> graph,
		std::shared_ptr<IPathFinder> pathfinder, bool isContinuous)
		: m_pGraph(graph)
		, m_CurrentNode(graph->GetNode(0))
		, m_TargetNode(graph->GetNode(0))
		, m_pPathFinder(pathfinder)
		, m_isContinuous(isContinuous)
		, m_QDistance{ 50 }
		, m_CurrentDirection{ Direction::NONE }
	{
		m_QDistance = m_pGraph->GetCellSize() * 1.6f;
	}

	void GridNavComponent::Start()
	{
		m_gameObject.lock()->SetLocalPosition(m_CurrentNode->GetPosition());
	}

	int GridNavComponent::GetNodeInDirection(Direction direction, int fromNodeIdx)
	{
		int to_node_idx = -1;
		switch (direction)
		{
		case Direction::UP:
			to_node_idx = m_pGraph->GetIndexUp(fromNodeIdx);
			break;
		case Direction::RIGHT:
			to_node_idx = m_pGraph->GetIndexRight(fromNodeIdx);
			break;
		case Direction::DOWN:
			to_node_idx = m_pGraph->GetIndexDown(fromNodeIdx);
			break;
		case Direction::LEFT:
			to_node_idx = m_pGraph->GetIndexLeft(fromNodeIdx);
			break;
		default:
			break;
		}

		return to_node_idx;
	}

	int dae::GridNavComponent::Move(Direction direction)
	{
		if (direction == m_CurrentDirection) return -1;

		// if we're moving, check if movement is possible from the next node instead from the one we came from
		int from_node_idx = m_Path.empty() ? m_CurrentNode->GetIndex() : m_TargetNode->GetIndex();

		int to_node_idx = -1;
		GraphNode* to_node;
		to_node = m_pGraph->GetNode(GetNodeInDirection(direction, from_node_idx));

		// but only if we're close enough
		const float dist_to_target = MathHelpers::glmDistanceSquared(m_gameObject.lock()->GetWorldPosition(), to_node->GetPosition());
		if (dist_to_target > m_QDistance * m_QDistance) return -1;
		to_node_idx = to_node->GetIndex();
		m_CurrentDirection = direction;
	
		if (m_Path.size() >= 1) m_Path.clear();

		auto from_node = m_pGraph->GetNode(from_node_idx);
		m_gameObject.lock()->GetTransform()->SetLocalPosition(from_node->GetPosition());

		to_node_idx = GetNodeInDirection(direction, from_node_idx);

		if (to_node_idx == -1) return -1;
			to_node = m_pGraph->GetNode(to_node_idx);
			AddMoveToPath(to_node);
			
		if (!m_isContinuous) return to_node_idx;

		int cached_next_idx = to_node_idx;
		while (to_node_idx != -1)
		{
			switch (direction)
			{
			case Direction::UP:
				to_node_idx = m_pGraph->GetIndexUp(to_node_idx);
				break;
			case Direction::RIGHT:
				to_node_idx = m_pGraph->GetIndexRight(to_node_idx);
				break;
			case Direction::DOWN:
				to_node_idx = m_pGraph->GetIndexDown(to_node_idx);
				break;
			case Direction::LEFT:
				to_node_idx = m_pGraph->GetIndexLeft(to_node_idx);
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

		return cached_next_idx;
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
				m_Path.erase(m_Path.begin()); // Remove the current node from the path

				// If we've reached the end of the path, stop moving
				if (m_Path.empty())
					return;
				else
				{
					// Otherwise, set the target node to the next node in the path
					m_TargetNode = m_Path.front();
					m_Path.erase(m_Path.begin());
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
				else
					m_gameObject.lock()->GetTransform()->SetLocalPosition(new_position);
			}
		}
		else
		{
			//if(MathHelpers::glmDistanceSquared(m_gameObject.lock()->GetTransform()->GetLocalPosition(), m_CurrentNode->GetPosition()) > m_QDistance * m_QDistance * 2)
				m_gameObject.lock()->GetTransform()->SetLocalPosition(m_CurrentNode->GetPosition());

		}


		const Uint8* pStates = SDL_GetKeyboardState(nullptr);
		if (pStates[SDL_SCANCODE_I])
		{
			std::cout << "stop!\n";
		}
	}

	void GridNavComponent::AddMoveToPath(GraphNode* toNode)
	{
		if (m_Path.empty() || m_Path.front() != toNode)
		{
			std::cout << "Adding move!\n";
			m_Path.push_back(toNode);
		}
	}
}