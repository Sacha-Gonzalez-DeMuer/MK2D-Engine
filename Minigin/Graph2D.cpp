#include "Graph2D.h"
#include "GraphNode.h"
#include "GraphConnection.h"
#include "MathHelpers.h"

namespace dae
{
	Graph2D::Graph2D(bool isDirectional)
		: IGraph(isDirectional)
	{
	}


	Graph2D::Graph2D(const Graph2D& other)
		: IGraph(other)
	{
	}


	std::shared_ptr<IGraph> Graph2D::Clone() const
	{
		return std::shared_ptr<Graph2D>(new Graph2D(*this));
	}

	glm::vec2 Graph2D::GetNodePos(GraphNode* pNode) const
	{
		return pNode->GetPosition();
	}


	int Graph2D::GetNodeIdxAtWorldPos(const glm::vec2& pos) const
	{
		float posErrorMargin = 1.5f;
		auto foundIt = std::find_if(this->m_Nodes.begin(), this->m_Nodes.end(),
			[pos, posErrorMargin, this](GraphNode* pNode)
			{
				return pNode->GetIndex() != invalid_node_index
					&& glm::length((pNode->GetPosition() - pos)) < std::pow(posErrorMargin * GetNodeRadius(), 2); });



		if (foundIt != this->m_Nodes.end())
			return (*foundIt)->GetIndex();
		else
			return invalid_node_index;
	}


	void Graph2D::SetConnectionCostsToDistance()
	{
		for (auto& connectionList : this->m_Connections)
		{
			for (auto& connection : connectionList)
			{
				auto posFrom = GetNodePos(connection->GetFrom());
				auto posTo = GetNodePos(connection->GetTo());
				connection->SetCost(abs(MathHelpers::glmDistanceSquared(posFrom, posTo)));
			}
		}
	}
}