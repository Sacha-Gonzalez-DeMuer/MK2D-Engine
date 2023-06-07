#pragma once
#include <vector>
#include <unordered_set>
#include <memory>
#include <glm/glm.hpp>

//
///* LIGHTWEIGHT IMPLEMENTATION OF THE ELITE FRAMEWORK GRAPHS */
//

namespace dae
{
	class GraphNode;
	class GraphConnection;
	class IGraph
	{
	public:
		using NodeVector = std::vector<GraphNode*>;
		using ConnectionList = std::list<GraphConnection*>;
		using ConnectionListVector = std::vector<ConnectionList>;
		const int invalid_node_index = -1;

	public:
		IGraph(bool isDirectionalGraph)
			: m_NextNodeIndex(0)
			, m_IsDirectionalGraph(isDirectionalGraph) {}

		IGraph(const IGraph& other);
		virtual ~IGraph();

		// Basic graph functionality
		// -------------------------
		GraphNode* GetNode(int idx) const;
		bool IsNodeValid(int idx) const;
		NodeVector GetAllNodes() const;

		GraphConnection* GetConnection(int from, int to) const;
		const ConnectionListVector& GetAllConnections() const { return m_Connections; }
		const ConnectionList& GetNodeConnections(int idx) const;
		const ConnectionList& GetNodeConnections(GraphNode* pNode) const;
		bool IsUniqueConnection(int from, int to) const;

		int GetNextFreeNodeIndex() const { return m_NextNodeIndex; }
		int AddNode(GraphNode* pNode);
		void RemoveNode(int node);

		void AddConnection(GraphConnection* pConnection);
		void RemoveConnection(int from, int to);
		void RemoveConnection(GraphConnection* pConnection);
		void RemoveConnections();

		// (Pure) virtuals to be override by specific graph types
		virtual glm::vec2 GetNodePos(GraphNode* pNode) const = 0;
		glm::vec2 GetNodePos(int idx) const { return GetNodePos(GetNode(idx)); }

		void Clear();

		// Provide the opportunity for derived classes to differentiate the conceptual position from the world position
		// Example: A grid position might consist of rows and columns, while the world position is expressed as a (x,y) coordinate
		virtual glm::vec2 GetNodeWorldPos(int idx) const;
		glm::vec2 GetNodeWorldPos(GraphNode* pNode) const;

		virtual int GetNodeIdxAtWorldPos(const glm::vec2& pos) const = 0;
		GraphNode* GetNodeAtWorldPos(const glm::vec2& pos) const;

		// Allow derived classes to implement a cloning function that returns a base class pointer
		virtual std::shared_ptr<IGraph> Clone() const { return nullptr; };

	protected:
		ConnectionListVector m_Connections{};
		NodeVector m_Nodes{};
		bool m_IsDirectionalGraph;

		// Called whenever the graph is modified, to be overriden by derived classes
		virtual void OnGraphModified(bool /*nrOfNodesChanged*/, bool /*nrOfConnectionsChanged*/) {}


	private:
		int m_NextNodeIndex;
	};


}
