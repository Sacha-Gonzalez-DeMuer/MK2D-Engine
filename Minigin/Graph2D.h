#pragma once
#include <iomanip>
#include <memory>
#include "GraphConnectionTypes.h"
#include "GraphNodeTypes.h"
#include "IGraph.h"

//
///* MODIFIED IMPLEMENTATION OF THE ELITE FRAMEWORK GRAPHS */
//

namespace dae
{
	template<class T_NodeType, class T_ConnectionType>
	class Graph2D : public IGraph<T_NodeType, T_ConnectionType>
	{
	public:
		using NodeVector = std::vector<T_NodeType*>;
		using ConnectionList = std::list<T_ConnectionType*>;
		using ConnectionListVector = std::vector<ConnectionList>;
		const int invalid_node_index = -1;
		const int default_node_radius = 5.f;
	public:
		Graph2D(bool isDirectional);
		Graph2D(const Graph2D& other);
		virtual ~Graph2D() = default;
		virtual std::shared_ptr<IGraph<T_NodeType, T_ConnectionType>> Clone() const override;

		virtual glm::vec2 GetNodePos(T_NodeType* pNode) const override { return pNode->GetPosition(); }
		float GetNodeRadius() const { return default_node_radius; }

		virtual int GetNodeIdxAtWorldPos(const glm::vec2& pos) const override;
		void SetConnectionCostsToDistance();

	private:
		int m_SelectedNodeIdx = -1;
	};

	template<class T_NodeType, class T_ConnectionType>
	Graph2D<T_NodeType, T_ConnectionType>::Graph2D(bool isDirectional)
		: IGraph<T_NodeType, T_ConnectionType>(isDirectional)
	{
	}

	template<class T_NodeType, class T_ConnectionType>
	inline Graph2D<T_NodeType, T_ConnectionType>::Graph2D(const Graph2D& other)
		: IGraph<T_NodeType, T_ConnectionType>(other)
	{
	}

	template<class T_NodeType, class T_ConnectionType>
	inline std::shared_ptr<IGraph<T_NodeType, T_ConnectionType>> Graph2D<T_NodeType, T_ConnectionType>::Clone() const
	{
		return std::shared_ptr<Graph2D>(new Graph2D(*this));
	}

	template<class T_NodeType, class T_ConnectionType>
	inline int Graph2D<T_NodeType, T_ConnectionType>::GetNodeIdxAtWorldPos(const glm::vec2& pos) const
	{
		float posErrorMargin = 1.5f;
		auto foundIt = std::find_if(this->m_Nodes.begin(), this->m_Nodes.end(),
			[pos, posErrorMargin, this](T_NodeType* pNode)
			{
				return pNode->GetIndex() != invalid_node_index 
					&& glm::length((pNode->GetPosition() - pos) ) < std::pow(posErrorMargin * GetNodeRadius(), 2); });



		if (foundIt != this->m_Nodes.end())
			return (*foundIt)->GetIndex();
		else
			return invalid_node_index;
	}

	template<class T_NodeType, class T_ConnectionType>
	void Graph2D<T_NodeType, T_ConnectionType>::SetConnectionCostsToDistance()
	{
		for (auto& connectionList : this->m_Connections)
		{
			for (auto& connection : connectionList)
			{
				auto posFrom = GetNodePos(connection->GetFrom());
				auto posTo = GetNodePos(connection->GetTo());
				connection->SetCost(abs(glm::distance(posFrom, posTo)));
			}
		}
	}

}