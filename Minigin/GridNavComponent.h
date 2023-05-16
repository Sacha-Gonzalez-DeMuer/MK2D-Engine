#pragma once
#include "Component.h"
#include "Datatypes.h"
#include <queue>
#include "GraphNode.h"
#include "GraphConnection.h"
#include "GridGraph.h"
#include "IPathFinder.h"
namespace dae
{
	class GridNavComponent : public Component
	{
	public:
		GridNavComponent() = delete;
		explicit GridNavComponent(std::shared_ptr<GridGraph> graph
			, std::shared_ptr<IPathFinder> pathfinder);
		virtual ~GridNavComponent() = default;

		void Move(Direction direction);
		void GoTo(GraphNode* node);

		bool IsMoving() const { return !m_Path.empty(); }

		GraphNode* GetCurrentNode() const { return m_CurrentNode; }
		void SetMovementSpeed(float speed) { m_MoveSpeed = speed; }

		virtual void Start() override;
		virtual void Update() override;

	protected:
		GraphNode* m_CurrentNode;
		GraphNode* m_TargetNode;
		float m_MoveSpeed{ 50 };

		void AddMoveToPath(GraphNode* toNode);
		float SqrDistanceToTarget() const;
	private:
		std::shared_ptr<GridGraph> m_pGraph;
		std::queue<GraphNode*> m_Path;
		std::shared_ptr<IPathFinder> m_pPathFinder;
	};
}
