#include "Commands.h"
#include "GameObject.h"
#include "Time.h"
#include "HealthComponent.h"

namespace dae
{
	void MoveCommand::Execute()
	{
		m_Object->GetTransform()->SetLocalPosition(m_Object->GetTransform()->GetLocalPosition() + m_Direction * m_Speed );
	}
	void KillCommand::Execute()
	{
		m_Health->Die();
	}
}
