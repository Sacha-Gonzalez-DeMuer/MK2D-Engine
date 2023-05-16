#pragma once
#include "../Minigin/GridNavComponent.h"

namespace dae
{
	class PacNavigator final : public GridNavComponent
	{
	public:
		PacNavigator() = delete;
		explicit PacNavigator(std::shared_ptr<GridGraph> graph
			, std::shared_ptr<IPathFinder> pathfinder);
		virtual ~PacNavigator() = default;

		virtual void Update() override;

		void SetDirection(Direction direction) { m_CurrentDirection = direction; }

	private:
		Direction m_CurrentDirection{ Direction::NONE };
	};
}
