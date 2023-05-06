#pragma once
#include <vector>

template <class T_NodeType, class T_ConnectionType>
class IPathFinder
{
public:
	virtual ~IPathFinder() = default;
	virtual std::vector<T_NodeType*> FindPath(T_NodeType* pStartNode, T_NodeType* pDestinationNode) = 0;
};

