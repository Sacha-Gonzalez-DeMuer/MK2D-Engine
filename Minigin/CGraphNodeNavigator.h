#pragma once
#include "Component.h"
#include "Datatypes.h"
#include "GraphTypes.h"
#include "GraphNodeTypes.h"
#include "GraphConnectionTypes.h"
#include "IPathFinder.h"

#include <queue>
namespace dae
{
	class Graph2DNavComponent final : public IPathFinder<GraphNode2D, GraphConnection2D>, public Component
	{
		using Graph2D = Graph2D<GraphNode2D, GraphConnection2D>;
		using GraphNode2D = GraphNode2D;
		using GraphConnection2D = GraphConnection2D;

	public:
		Graph2DNavComponent() = delete;
		explicit Graph2DNavComponent(std::shared_ptr<Graph2D> graph, std::shared_ptr<GraphNode2D> node);
		virtual ~Graph2DNavComponent() = default;

		// adds command to be executed to the queue
		void Move(const Direction& direction);
		void GoTo(std::shared_ptr<GraphNode2D> node);

		virtual void Update() override;

	private:
		std::queue<Direction> m_Commands;
		std::shared_ptr<Graph2D> m_pGraph;
		std::shared_ptr<GraphNode2D> m_CurrentNode;
		std::shared_ptr<GraphNode2D> m_TargetNode;
		std::queue<std::shared_ptr<GraphNode2D>> m_Path;
	};
}
