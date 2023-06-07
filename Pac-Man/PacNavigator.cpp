#include "PacNavigator.h"
#include "PacGrid.h"
#include "MathHelpers.h"
#include "GameTime.h"
#include "GraphNode.h"
#include "GraphConnection.h"
#include "GridGraph.h"
#include <memory>
#include <iostream>
#include <SDL.h>
#include "IPathFinder.h"
#include "Datatypes.h"


dae::PacNavigator::PacNavigator(std::shared_ptr<PacGrid> graph, std::shared_ptr<IPathFinder> pathfinder)
	: m_pGraph{ graph }, m_pPathFinder{ pathfinder }, m_pPacGrid{ graph }, m_CurrentDirection{ Direction::NONE }
	, m_TargetNode{nullptr}, m_CurrentNode{nullptr}, m_QDistance{50}
{
	m_CurrentNode = graph->GetNode(graph->GetSpawnIdx());
	//m_TargetNode = graph->GetNode(graph->GetSpawnIdx());
	m_QDistance = m_pGraph->GetCellSize() * 1.5f;
}

void dae::PacNavigator::Start()
{
	m_gameObject.lock()->SetLocalPosition(m_CurrentNode->GetPosition());
}

void dae::PacNavigator::Update()
{
	if (!m_TargetNode)
		return;

	constexpr float epsilon = 1.0f;
	float distance_to_target = MathHelpers::glmDistanceSquared(m_gameObject.lock()->GetWorldPosition(), m_TargetNode->GetPosition());
	// If we're close enough to the target node, snap to it and move on to the next one
	if (abs(distance_to_target) <= epsilon)
	{
		m_gameObject.lock()->GetTransform()->SetLocalPosition(m_TargetNode->GetPosition());
		m_CurrentNode = m_TargetNode;
		m_TargetNode = nullptr;

		OnArriveAtTarget.Invoke();

		// find next node using direction queue
		if (!m_DirectionQueue.empty())
		{
			auto next_node_idx = GetNodeInDirection(m_DirectionQueue.front(), m_CurrentNode->GetIndex());

			if (IsValid(next_node_idx))
			{
				m_CurrentDirection = m_DirectionQueue.front();
				m_DirectionQueue.pop();
				m_TargetNode = m_pGraph->GetNode(next_node_idx);
			}
			else
			{
				m_CurrentDirection = Direction::NONE;
				m_TargetNode = nullptr;
				return;
			}
		} 
		else // If no more inputs are queued
		{
			// check if we can continue in the same direction
			auto next_node_idx = GetNodeInDirection(m_CurrentDirection, m_CurrentNode->GetIndex());
			if (IsValid(next_node_idx))
			{
				m_TargetNode = m_pGraph->GetNode(next_node_idx);
			}
			else
			{
				// otherwise, stop moving
				m_CurrentDirection = Direction::NONE;
				m_TargetNode = nullptr;
				return;
			}
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

bool dae::PacNavigator::Move(Direction direction)
{
	if (direction == m_CurrentDirection) return false;

	if (!m_TargetNode)
	{
		auto target = GetNodeInDirection(direction, m_CurrentNode->GetIndex());
		if (IsValid(target))
		{
			m_TargetNode = m_pGraph->GetNode(target);
			return true;
		}
		else return false;
	}

	if(!m_DirectionQueue.empty())
		m_DirectionQueue = std::queue<Direction>();

	m_DirectionQueue.emplace(direction);
	return true;
}

void dae::PacNavigator::SetPathToNode(int nodeIdx)
{
	if(!IsValid(nodeIdx)) return;

	const auto& path = m_pPathFinder->FindPath(m_CurrentNode, m_pGraph->GetNode(nodeIdx));
	if (path.empty()) return;

	m_DirectionQueue = std::queue<Direction>();
	for (size_t i = 0; i < path.size() - 1; ++i)
	{
		auto from = path[i];
		auto to = path[i + 1];
		auto direction = m_pGraph->GetConnectionDirection(from->GetIndex(), to->GetIndex());
		m_DirectionQueue.emplace(direction);
	}
}

void dae::PacNavigator::SetPathToNode(const glm::vec2& position)
{
	auto nodeAtPos = m_pGraph->GetNodeAtWorldPos(position);
	SetPathToNode(nodeAtPos->GetIndex());
}

int dae::PacNavigator::GetNodeInDirection(Direction direction, int fromNodeIdx)
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

float dae::PacNavigator::SqrDistanceToTarget() const
{
	return MathHelpers::glmDistanceSquared(m_gameObject.lock()->GetWorldPosition(), m_TargetNode->GetPosition());
}


bool dae::PacNavigator::IsValid(int i)
{
	return (i != -1 && m_pPacGrid->GetPacNodeInfo(i).type != PacGridData::PacNodeType::Wall);
}
