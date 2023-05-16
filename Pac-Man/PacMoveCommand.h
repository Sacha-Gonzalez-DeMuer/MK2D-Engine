#pragma once
#include "../Minigin/Commands.h"
#include "PacNavigator.h"

namespace dae
{
	class PacMoveCommand final : public Command
	{
	public:
		PacMoveCommand() = delete;
		explicit PacMoveCommand(std::shared_ptr<PacNavigator> navigator, Direction direction);
		virtual ~PacMoveCommand() = default;
		virtual void Execute() override;

	private:
		std::shared_ptr<PacNavigator> m_Navigator;
		Direction m_Direction;
	};
}
