#include "PacCommands.h"
#include "PacNavigator.h"
#include "PacGameManager.h"

namespace dae
{
	dae::PacMoveCommand::PacMoveCommand(std::shared_ptr<PacNavigator> navigator, Direction direction)
		: m_Navigator(navigator)
		, m_Direction(direction)
	{
	}

	void dae::PacMoveCommand::Execute()
	{
		m_Navigator->Move(m_Direction);
	}

	void PacSkipLevelCommand::Execute()
	{
		PacGameManager::Get().LoadNextLevel(true);
	}
}
