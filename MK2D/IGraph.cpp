#include "IGraph.h"
#include "GraphNode.h"
#include "GraphConnection.h"


namespace dae
{

	IGraph::IGraph(const IGraph& other)
	{
		for (auto& n : m_Nodes)
		{
			delete n;
			n = nullptr;
		}

		for (auto& connectionList : m_Connections)
		{
			for (auto& connection : connectionList)
			{
				delete connection;
				connection = nullptr;
			}
		};

		for (auto n : other.m_Nodes)
			m_Nodes.push_back(new GraphNode(*n));

		for (auto cList : other.m_Connections)
		{
			ConnectionList newList;
			for (auto c : cList)
				newList.push_back(new GraphConnection(*c));
			this->m_Connections.push_back(newList);
		}

		m_IsDirectionalGraph = other.m_IsDirectionalGraph;
		m_NextNodeIndex = other.m_NextNodeIndex;
	}

	IGraph::~IGraph()
	{
		Clear();
	}


	GraphNode* IGraph::GetNode(int idx) const
	{
		assert((idx < (int)m_Nodes.size()) && (idx >= 0) && "<Graph::GetNode>: invalid index");

		return m_Nodes[idx];
	}


	bool IGraph::IsNodeValid(int idx) const
	{
		return (idx < (int)m_Nodes.size() && idx != invalid_node_index);
	}


	GraphConnection* IGraph::GetConnection(int from, int to) const
	{
		assert((from < (int)m_Nodes.size()) &&
			(from >= 0) &&
			m_Nodes[from]->GetIndex() != invalid_node_index &&
			"<Graph::GetConnection>: invalid 'from' index");

		assert((to < (int)m_Nodes.size()) &&
			(to >= 0) &&
			m_Nodes[to]->GetIndex() != invalid_node_index &&
			"<Graph::GetConnection>: invalid 'to' index");

		for (auto c : m_Connections[from])
		{
			if (c && c->GetTo() == to)
				return c;
		}

		return nullptr;
	}


	std::vector<GraphNode*> IGraph::GetAllNodes() const
	{
		std::vector<GraphNode*> activeNodes{};
		for (auto n : m_Nodes)
			if (n->GetIndex() != invalid_node_index)
				activeNodes.push_back(n);

		return activeNodes;
	}


	const std::list<GraphConnection*>& IGraph::GetNodeConnections(int idx) const
	{
		//assert((idx < (int)m_Nodes.size()) && (idx >= 0) && "<Graph::GetNode>: invalid index");

		return m_Connections[idx];
	}

	const IGraph::ConnectionList& IGraph::GetNodeConnections(GraphNode* pNode) const
	{
		return GetNodeConnections(pNode->GetIndex());
	}


	bool IGraph::IsUniqueConnection(int from, int to) const
	{
		for (auto c : m_Connections[from])
		{
			if (c->GetTo() == to)
			{
				return false;
			}
		}

		return true;
	}


	void IGraph::Clear()
	{
		for (auto& n : m_Nodes)
		{
			delete n;
			n = nullptr;
		}
		m_Nodes.clear();

		for (auto& connectionList : m_Connections)
		{
			for (auto& connection : connectionList)
			{
				delete connection;
				connection = nullptr;
			}
		}
		m_Connections.clear();

		m_NextNodeIndex = 0;
	}

	glm::vec2 IGraph::GetNodeWorldPos(int idx) const
	{
		return GetNodePos(idx);
	}

	glm::vec2 IGraph::GetNodeWorldPos(GraphNode* pNode) const
	{
		return GetNodeWorldPos(pNode->GetIndex());
	}

	GraphNode* IGraph::GetNodeAtWorldPos(const glm::vec2& pos) const
	{
		return IsNodeValid(GetNodeIdxAtWorldPos(pos)) ? GetNode(GetNodeIdxAtWorldPos(pos)) : nullptr;
	}



	void IGraph::RemoveNode(int idx)
	{
		//Removes pNode by setting it's index to invalid_node_index 
		//This prevents the other indices from needing to be changed, however it can be reused when adding a new pNode with that index

		//assert(idx < (int)m_Nodes.size() && "<Graph::RemoveNode>: invalid node index");

		//set this pNode's index to invalid_node_index
		m_Nodes[idx]->SetIndex(invalid_node_index);

		bool hadConnections = false;

		//if the graph is not directed remove all connections leading to this pNode and then
		//clear the connections leading from the pNode
		if (!m_IsDirectionalGraph)
		{
			//visit each neighbour and erase any connections leading to this pNode
			for (auto currentConnection = m_Connections[idx].begin();
				currentConnection != m_Connections[idx].end();
				++currentConnection)
			{
				for (auto currentEdgeOnToNode = m_Connections[(*currentConnection)->GetTo()].begin();
					currentEdgeOnToNode != m_Connections[(*currentConnection)->GetTo()].end();
					++currentEdgeOnToNode)
				{
					if ((*currentEdgeOnToNode)->GetTo() == idx)
					{
						hadConnections = true;

						auto conPtr = *currentEdgeOnToNode;
						currentEdgeOnToNode = m_Connections[(*currentConnection)->GetTo()].erase(currentEdgeOnToNode);
						delete conPtr;
						conPtr = nullptr;

						break;
					}
				}
			}
		}

		//finally, clear this pNode's connections
		for (auto& connection : m_Connections[idx])
		{
			hadConnections = true;
			delete connection;
			connection = nullptr;
		}
		m_Connections[idx].clear();

		OnGraphModified(true, hadConnections);
	}


	int IGraph::AddNode(GraphNode* pNode)
	{
		if (pNode->GetIndex() < (int)m_Nodes.size())
		{
			//make sure the client is not trying to add a pNode with the same ID as
			//a currently active pNode
			//assert(m_Nodes[pNode->GetIndex()]->GetIndex() == invalid_node_index &&
			//	"<Graph::AddNode>: Attempting to add a node with a duplicate ID");

			m_Nodes[pNode->GetIndex()] = pNode;

			OnGraphModified(true, false);
			return m_NextNodeIndex;
		}
		else
		{
			//make sure the new pNode has been indexed correctly
			//assert(pNode->GetIndex() == m_NextNodeIndex && "<Graph::AddNode>:invalid index");

			m_Nodes.push_back(pNode);
			m_Connections.push_back(ConnectionList());

			OnGraphModified(true, false);
			return m_NextNodeIndex++;
		}
	}



	void IGraph::AddConnection(GraphConnection* pConnection)
	{
		//first make sure the from and to nodes exist within the graph 
		//assert((pConnection->GetFrom() < m_NextNodeIndex) && (pConnection->GetTo() < m_NextNodeIndex) && (pConnection->GetTo() != pConnection->GetFrom()) &&
		//	"<Graph::AddConnection>: invalid node index");

		//make sure both nodes are active before adding the pConnection
		if ((m_Nodes[pConnection->GetTo()]->GetIndex() != invalid_node_index) &&
			(m_Nodes[pConnection->GetFrom()]->GetIndex() != invalid_node_index))
		{
			//add the pConnection, first making sure it is unique
			if (!this->IsUniqueConnection(pConnection->GetFrom(), pConnection->GetTo()))
			{
				return;
			}

			this->m_Connections[pConnection->GetFrom()].push_back(pConnection);

			//if the graph is undirected we must add another pConnection in the opposite
			//direction
			if (!m_IsDirectionalGraph)
			{
				//check to make sure the pConnection is unique before adding
				if (this->IsUniqueConnection(pConnection->GetTo(), pConnection->GetFrom()))
				{
					GraphConnection* oppositeDirEdge = new GraphConnection();

					oppositeDirEdge->SetCost(pConnection->GetCost());
					oppositeDirEdge->SetTo(pConnection->GetFrom());
					oppositeDirEdge->SetFrom(pConnection->GetTo());

					m_Connections[pConnection->GetTo()].push_back(oppositeDirEdge);
				}
			}
		}

		OnGraphModified(false, true);
	}


	void IGraph::RemoveConnection(int from, int to)
	{
		//assert((from < (int)m_Nodes.size()) && (to < (int)m_Nodes.size()) &&
		//	"<Graph::RemoveConnection>:invalid node index");

		auto conFromTo = GetConnection(from, to);
		auto conToFrom = GetConnection(to, from);
		if (!m_IsDirectionalGraph)
		{
			for (auto curEdge = m_Connections[to].begin();
				curEdge != m_Connections[to].end();
				++curEdge)
			{
				if ((*curEdge)->GetTo() == from)
				{
					curEdge = m_Connections[to].erase(curEdge);
					break;
				}
			}
		}

		for (auto curEdge = m_Connections[from].begin();
			curEdge != m_Connections[from].end();
			++curEdge)
		{
			if ((*curEdge)->GetTo() == to)
			{
				curEdge = m_Connections[from].erase(curEdge);
				break;
			}
		}

		delete conFromTo;
		conFromTo = nullptr;
		delete conToFrom;
		conToFrom = nullptr;

		OnGraphModified(false, true);
	}


	void IGraph::RemoveConnection(GraphConnection* pConnection)
	{
		RemoveConnection(pConnection->GetFrom(), pConnection->GetTo());
	}

	void IGraph::RemoveConnections()
	{
		for (auto& connectionList : m_Connections)
			connectionList.clear();
	}
}
