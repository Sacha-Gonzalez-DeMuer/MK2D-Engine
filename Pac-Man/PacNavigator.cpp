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

using namespace dae::PacData;

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

		// find next node using direction queue
		if (!m_DirectionQueue.empty())
		{
			auto next_node_idx = GetNodeInDirection(m_DirectionQueue.front(), m_CurrentNode->GetIndex());

			if (IsValid(next_node_idx))
			{
				m_CurrentDirection= m_DirectionQueue.front();
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
	
	//else
	//{
	//	//if(MathHelpers::glmDistanceSquared(m_gameObject.lock()->GetTransform()->GetLocalPosition(), m_CurrentNode->GetPosition()) > m_QDistance * m_QDistance * 2)
	//	m_gameObject.lock()->GetTransform()->SetLocalPosition(m_CurrentNode->GetPosition());
	//	m_CurrentDirection = Direction::NONE;
	//}

}

void dae::PacNavigator::Move(Direction direction)
{
	if (direction == m_CurrentDirection) return;

	if (!m_TargetNode)
	{
		auto target = GetNodeInDirection(direction, m_CurrentNode->GetIndex());
		if (IsValid(target))
		{
			m_TargetNode = m_pGraph->GetNode(target);
			return;
		}
	}

	if(!m_DirectionQueue.empty())
		m_DirectionQueue = std::queue<Direction>();

	m_DirectionQueue.emplace(direction);
	
	//// check if move is valid
	//if (m_pPacGrid->GetPacNodeInfo(to_node->GetIndex()).type == PacGridData::PacNodeType::Wall)
	//	return;

	//if (m_Path.empty()) m_TargetNode = to_node;

	//if (m_Path.size() >= 1)
	//	m_Path = std::queue<GraphNode*>();

	//AddMoveToPath(to_node);

	// continuous movement
	//int to_node_idx = to_node->GetIndex();
	//while (to_node_idx != -1)
	//{
	//	switch (direction)
	//	{
	//	case Direction::UP:
	//		to_node_idx = m_pGraph->GetIndexUp(to_node_idx);
	//		break;
	//	case Direction::RIGHT:
	//		to_node_idx = m_pGraph->GetIndexRight(to_node_idx);
	//		break;
	//	case Direction::DOWN:
	//		to_node_idx = m_pGraph->GetIndexDown(to_node_idx);
	//		break;
	//	case Direction::LEFT:
	//		to_node_idx = m_pGraph->GetIndexLeft(to_node_idx);
	//		break;
	//	default:
	//		break;
	//	}
	//
	//	if (to_node_idx != -1)
	//	{
	//		// Pac Navigation
	//		if (m_pPacGrid->GetPacNodeInfo(to_node_idx).type == PacGridData::PacNodeType::Wall)
	//			return;
	//
	//		to_node = m_pGraph->GetNode(to_node_idx);
	//		AddMoveToPath(to_node);
	//	}
	//}

	//// if we're moving, check if movement is possible from the next node instead from the one we came from
	//const float dist_to_current = MathHelpers::glmDistanceSquared(m_gameObject.lock()->GetWorldPosition(), m_CurrentNode->GetPosition());
	//const float dist_to_target = MathHelpers::glmDistanceSquared(m_gameObject.lock()->GetWorldPosition(), m_TargetNode->GetPosition());

	//if (dist_to_target > m_QDistance * m_QDistance && !is_opposite) return;

	//m_CurrentDirection = direction;

	//auto from_node = dist_to_current < dist_to_target ? m_CurrentNode : m_TargetNode;
	//GraphNode* to_node = m_pGraph->GetNode(GetNodeInDirection(direction, from_node->GetIndex()));


	//if (m_Path.size() >= 1)
	//{
	//	m_Path.clear();
	//};

	//if(dist_to_target < (m_QDistance * m_QDistance) * .01f) m_gameObject.lock()->GetTransform()->SetLocalPosition(from_node->GetPosition());  //snap 

	//to_node_idx = GetNodeInDirection(direction, from_node->GetIndex());

	//if (to_node_idx == -1) return;

	////// Pac Navigation
	//if (m_pPacGrid->GetPacNodeInfo(to_node_idx).type == PacGridData::PacNodeType::Wall)
	//	return;


	////////////////////
	//

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

void dae::PacNavigator::AddMoveToPath(GraphNode* toNode)
{
	if (m_Path.empty() || m_Path.front() != toNode)
	{
		m_Path.emplace(toNode);
	}
}

bool dae::PacNavigator::IsValid(int i)
{
	return (i != -1 && m_pPacGrid->GetPacNodeInfo(i).type != PacGridData::PacNodeType::Wall);
}
