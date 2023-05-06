#pragma once
#include "Component.h"
#include "GridGraph.h"
#include "Datatypes.h"
#include "IPathFinder.h"
#include <queue>
#include <memory>

class GraphNode2D;
class GraphConnection2D;
namespace dae
{
	class GridNavComponent final : public Component
	{
	public:
		GridNavComponent() = delete;
		explicit GridNavComponent(std::shared_ptr<GridGraph<GraphNode2D, GraphConnection2D>> graph
			, std::shared_ptr<IPathFinder<GraphNode2D, GraphConnection2D>> pathfinder);
		virtual ~GridNavComponent() = default;

		// adds command to be executed to the queue
		void Move(Direction direction);
		void GoTo(std::shared_ptr<GraphNode2D> node);

		virtual void Update() override;

	private:
		std::queue<Direction> m_Commands;
		std::shared_ptr<GridGraph<GraphNode2D, GraphConnection2D>> m_pGraph;
		std::shared_ptr<GraphNode2D> m_CurrentNode;
		std::shared_ptr<GraphNode2D> m_TargetNode;
		std::queue<std::shared_ptr<GraphNode2D>> m_Path;
		std::shared_ptr<IPathFinder<GraphNode2D, GraphConnection2D>> m_pPathFinder;
	};
}
