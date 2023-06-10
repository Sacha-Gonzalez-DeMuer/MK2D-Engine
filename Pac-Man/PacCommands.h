#pragma once
#include "Commands.h"
#include "PacNavigator.h"
#include "Datatypes.h"

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

	class PacSkipLevelCommand final : public Command
	{
	public:
		PacSkipLevelCommand() = default;
		virtual ~PacSkipLevelCommand() = default;
		virtual void Execute() override;
	};
}
