#include "AStarPathFinder.h"
#include "GraphConnection.h"
#include "GraphNode.h"
#include "IGraph.h"
#include "Heuristic.h"
#include <algorithm>

namespace dae
{
	AStarPathFinder::AStarPathFinder(std::shared_ptr<IGraph> graph)
		: m_pGraph{ graph }
	{
	}

	std::vector<GraphNode*> AStarPathFinder::FindPath(GraphNode* pStartNode, GraphNode* pDestinationNode)
	{
		if (pStartNode == pDestinationNode) return {};
		//2.D wrong check english slides! -> "Connection to the connections endNode => This connection already points to a previously visited node
		std::vector<GraphNode*> path;
		std::vector<NodeRecord> openList{}; //connections to be checked
		std::vector<NodeRecord> closedList{}; //connections already checked
		NodeRecord currentRecord{};

		const NodeRecord startRecord{ pStartNode, nullptr, GetHeuristicCost(pStartNode, pDestinationNode) };
		openList.push_back(startRecord);

		//--------------STEP 1 COMPLETE---------------\\

		//2.
		while (!openList.empty())
		{
			//get connection with lowest F score
			currentRecord = GetLowestFScoreConnection(openList);

			//check if that connection leads to the end node. if so, end loop
			if (currentRecord.pConnection && currentRecord.pConnection->GetTo() == pDestinationNode->GetIndex()) break;

			//else get all connections current node, loop over them
			const auto currentRecordConnections{ m_pGraph->GetNodeConnections(currentRecord.pNode) };

			//For each of the connections…
			for (auto& connectionFromCurrentRecord : currentRecordConnections)
			{
				//---------------STEP 2.1 COMPLETE-----------------\\
				// 
				//calculate g-cost so far
				GraphNode* nextNode{ m_pGraph->GetNode(connectionFromCurrentRecord->GetTo()) };
				float gCost = currentRecord.costSoFar + 1 + GetHeuristicCost(currentRecord.pNode, pDestinationNode) + connectionFromCurrentRecord->GetCost();

				//Check if any of those connections lead to a node already on the closed list
				bool isExistingConnectionCheaper{};
				for (auto& existingRecord : closedList)
				{
					if (connectionFromCurrentRecord->GetTo() == existingRecord.pNode->GetIndex()) // if connection leads to already checked connection
					{
						if (existingRecord.costSoFar < gCost) //Check if the already existing connection is cheaper 
						{
							//don't execute 2.F..
							isExistingConnectionCheaper = true;
							break;
						}
						else
						{
							closedList.erase(std::remove(closedList.begin(), closedList.end(), existingRecord));
						}

					}
				}

				//check if any of those connections lead to a node already on the open list
				for (auto& existingRecord : openList)
				{
					//if a connection to the connections endNode already exist in the open list
					if (connectionFromCurrentRecord->GetTo() == existingRecord.pNode->GetIndex())
					{
						if (existingRecord.costSoFar < gCost) //Check if the already existing connection is cheaper 
						{
							//don't execute 2.F..
							isExistingConnectionCheaper = true;
							break;
						}
						else
						{
							openList.erase(std::remove(openList.begin(), openList.end(), existingRecord));
						}
					}
				}

				if (isExistingConnectionCheaper) continue;

				//2.F
				//create new nodeRecord, add it to openList
				NodeRecord newNodeRecord{  };
				newNodeRecord.pNode = nextNode;
				newNodeRecord.costSoFar = gCost;
				newNodeRecord.estimatedTotalCost = gCost + GetHeuristicCost(nextNode, pDestinationNode);
				newNodeRecord.pConnection = connectionFromCurrentRecord;
				openList.emplace_back(newNodeRecord);
			}

			//move current record to closed list
			closedList.push_back(currentRecord);
			openList.erase(std::remove(openList.begin(), openList.end(), currentRecord));

		}

		//3. Reconstruct path from last connection to start node
		while (currentRecord.pNode != pStartNode)
		{
			path.emplace_back(currentRecord.pNode);
			//look in the closedList for a record where pNode == currentRecord connection startNode
			for (auto& record : closedList)
			{
				if (record.pNode->GetIndex() == currentRecord.pConnection->GetFrom()) // found record
				{
					currentRecord = record; //set new currentRecord to found record
					break;
				}
			}
		}

		path.emplace_back(pStartNode);
		std::reverse(path.begin(), path.end());
		return path;
	}

	float AStarPathFinder::GetHeuristicCost(GraphNode* pStartNode, GraphNode* pEndNode) const
	{
		glm::vec2 toDestination = m_pGraph->GetNodePos(pEndNode) - m_pGraph->GetNodePos(pStartNode);
		return HeuristicFunctions::SqrtEuclidean(abs(toDestination.x), abs(toDestination.y));
	}

	AStarPathFinder::NodeRecord AStarPathFinder::GetLowestFScoreConnection(std::vector<NodeRecord> fromList) const
	{
		return *std::min_element(fromList.begin(), fromList.end());
	}

}
