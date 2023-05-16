#include "PacMoveCommand.h"

dae::PacMoveCommand::PacMoveCommand(std::shared_ptr<PacNavigator> navigator, Direction direction)
	: m_Navigator(navigator)
	, m_Direction(direction)
{
}

void dae::PacMoveCommand::Execute()
{
	m_Navigator->SetDirection(m_Direction);
}
