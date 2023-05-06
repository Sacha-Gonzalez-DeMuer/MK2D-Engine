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
	class GridNavComponent final : public Component
	{
	public:
		GridNavComponent() = delete;
		explicit GridNavComponent(std::shared_ptr<GridGraph> graph
			, std::shared_ptr<IPathFinder> pathfinder);
		virtual ~GridNavComponent() = default;

		// adds command to be executed to the queue
		void Move(Direction direction);
		void GoTo(std::shared_ptr<GraphNode> node);

		virtual void Start() override;
		virtual void Update() override;

	private:
		std::queue<Direction> m_Commands;
		std::shared_ptr<GridGraph> m_pGraph;
		GraphNode* m_CurrentNode;
		GraphNode* m_TargetNode;
		std::queue<std::shared_ptr<GraphNode>> m_Path;
		std::shared_ptr<IPathFinder> m_pPathFinder;
	};
}
