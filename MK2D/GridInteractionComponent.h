#pragma once
#include "Component.h"

class GridGraph;

namespace dae
{
	class GridInteractionComponent final : public Component
	{
	public:
		GridInteractionComponent(std::shared_ptr<GridGraph> grid);
		~GridInteractionComponent() = default;
		void Update() override;

	private:
		std::shared_ptr<GridGraph> m_pGridGraph;
	};

}

