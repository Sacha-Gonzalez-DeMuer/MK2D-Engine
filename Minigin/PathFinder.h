#pragma once
#include "Component.h"
class PathFinder : public Component
{
	public:
	PathFinder() = default;
	virtual ~PathFinder() = default;
	virtual void Update() override = 0;

	private:

};

