#pragma once
#include "Component.h"
#include <queue>
#include "Delegate.h"
#include "Datatypes.h"

namespace dae
{
	class PacGrid;
	class GridGraph;
	class IPathFinder;
	class GraphNode;
	//forward declare templated delegate class
	class PacNavigator final : public Component
	{
	public:
		PacNavigator() = delete;
		explicit PacNavigator(std::shared_ptr<PacGrid> graph
			,std::shared_ptr<IPathFinder> pathfinder);
		virtual ~PacNavigator() = default;

		void Start() override;
		void Update() override;
		GraphNode* GetCurrentNode() const { return m_CurrentNode; }
		void SetMovementSpeed(float speed) { m_MoveSpeed = speed; }
		bool Move(Direction direction);
		void SetPathToNode(int nodeIdx);
		void SetPathToNode(const glm::vec2& position);

		bool HasTarget() const {return m_TargetNode != nullptr;}

		Delegate<> OnArriveAtTarget;
		Direction GetCurrentDirection() const { return m_CurrentDirection; }
		bool HasPath() const { return !m_DirectionQueue.empty(); }
	private:
		std::shared_ptr<PacGrid> m_pPacGrid;
		std::shared_ptr<GridGraph> m_pGraph;
		std::shared_ptr<IPathFinder> m_pPathFinder;

		GraphNode* m_CurrentNode;
		GraphNode* m_TargetNode;
		std::queue<Direction> m_DirectionQueue;
		Direction m_CurrentDirection;

		float m_MoveSpeed{ 50 };
		float m_QDistance{}; // distance at which a move is allowed to be queued

		int GetNodeInDirection(Direction direction, int fromNodeIdx);
		float SqrDistanceToTarget() const;

		bool IsValid(int i);


	};
}


