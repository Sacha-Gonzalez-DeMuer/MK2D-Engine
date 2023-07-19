#pragma once
#include <iomanip>
#include <memory>
#include "IGraph.h"

//
///* MODIFIED IMPLEMENTATION OF THE ELITE FRAMEWORK GRAPHS */
//

namespace dae
{
	class GraphNode;
	class GraphConnection;
	class Graph2D : public IGraph
	{
	public:
		using NodeVector = std::vector<GraphNode*>;
		using ConnectionList = std::list<GraphConnection*>;
		using ConnectionListVector = std::vector<ConnectionList>;
		const int invalid_node_index = -1;
		const float default_node_radius = 5.f;
	public:
		Graph2D(bool isDirectional);
		Graph2D(const Graph2D& other);
		virtual ~Graph2D() = default;
		virtual std::shared_ptr<IGraph> Clone() const override;

		using IGraph::GetNodePos;
		virtual glm::vec2 GetNodePos(GraphNode* pNode) const override;
		float GetNodeRadius() const { return default_node_radius; }

		virtual int GetNodeIdxAtWorldPos(const glm::vec2& pos) const override;
		void SetConnectionCostsToDistance();

	private:
		int m_SelectedNodeIdx = -1;
	};

}