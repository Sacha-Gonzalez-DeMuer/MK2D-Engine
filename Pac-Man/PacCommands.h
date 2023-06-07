#pragma once
#include "../Minigin/Commands.h"
#include "PacNavigator.h"
#include "PacData.h"

namespace dae
{
	class PacMoveCommand final : public Command
	{
	public:
		PacMoveCommand() = delete;
		explicit PacMoveCommand(std::shared_ptr<PacNavigator> navigator, PacData::Direction direction);
		virtual ~PacMoveCommand() = default;
		virtual void Execute() override;

	private:
		std::shared_ptr<PacNavigator> m_Navigator;
		PacData::Direction m_Direction;
	};
}
