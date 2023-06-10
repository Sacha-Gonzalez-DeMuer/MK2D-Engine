#pragma once
#include "Component.h"
#include "Delegate.h"
#include "Datatypes.h"
#include "PacData.h"
#include <queue>

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
		explicit PacNavigator(std::shared_ptr<PacGrid> graph
			,std::shared_ptr<IPathFinder> pathfinder = nullptr);
		PacNavigator() = delete;
		PacNavigator(const PacNavigator& other) = delete;
		PacNavigator(PacNavigator&& other) noexcept = delete;
		PacNavigator& operator=(const PacNavigator& other) = delete;
		PacNavigator& operator=(PacNavigator&& other) noexcept = delete;
		virtual ~PacNavigator() = default;

		void Start() override;
		void Update() override;
		GraphNode* GetCurrentNode() const { return m_CurrentNode; }
		void SetMovementSpeed(float speed) { m_MoveSpeed = speed; }
		bool Move(Direction direction);
		std::vector<Direction> GetLegalMoves() const;
		void SetPathToNode(int nodeIdx);
		void SetPathToNode(const glm::vec2& position);

		bool HasTarget() const {return m_TargetNode != nullptr;}
		Direction GetCurrentDirection() const { return m_CurrentDirection; }
		bool HasPath() const { return !m_DirectionQueue.empty(); }
		bool AreOpposites(Direction first, Direction second) const;

		Delegate<int, std::shared_ptr<PacGrid>> OnArriveAtTarget;
		Delegate<> OnDotCollected;
		Delegate<> OnPowerUpFound;

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

		int GetNodeInDirection(Direction direction, int fromNodeIdx) const;
		float SqrDistanceToTarget() const;

		bool IsValid(int i) const;
	};
}


