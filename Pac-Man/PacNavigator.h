#pragma once
#include "Component.h"
#include "PacData.h"
#include <queue>

namespace dae
{
	class PacGrid;
	class GridGraph;
	class IPathFinder;
	class GraphNode;
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
		void Move(PacData::Direction direction);
		void SetDirection(PacData::Direction direction);

		bool IsMoving() const { return !m_Path.empty(); }

	private:
		std::shared_ptr<PacGrid> m_pPacGrid;
		std::shared_ptr<GridGraph> m_pGraph;
		std::shared_ptr<IPathFinder> m_pPathFinder;

		GraphNode* m_CurrentNode;
		GraphNode* m_TargetNode;
		std::queue<GraphNode*> m_Path;
		std::queue<PacData::Direction> m_DirectionQueue;
		PacData::Direction m_CurrentDirection;

		float m_MoveSpeed{ 50 };
		float m_QDistance{}; // distance at which a move is allowed to be queued

		int GetNodeInDirection(PacData::Direction direction, int fromNodeIdx);
		float SqrDistanceToTarget() const;
		void AddMoveToPath(GraphNode* toNode);

		bool IsValid(int i);
	};
}


