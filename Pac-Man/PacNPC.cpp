#include "PacNPC.h"
#include "PacNavigator.h"
#include "MathHelpers.h"
#include "PacData.h"
#include "ICollider.h"

#include "PacNPCState.h"
#include "PacNPCWander.h"
#include "PacNPCChase.h"
#include <iostream>

namespace dae
{
	PacNPC::PacNPC(std::shared_ptr<PacNavigator> pNavigator)
		: m_pNavigator(pNavigator),m_Vulnerable{false}, m_State{std::make_shared<PacNPCWander>()}
	{
	}

	void dae::PacNPC::Update()
	{
		if (!m_pNavigator->HasTarget() || m_pNavigator->GetCurrentDirection() == Direction::NONE)
			m_State->OnArrive(*this);
	}

	void dae::PacNPC::SetTarget(std::shared_ptr<GameObject> target)
	{
		m_pTarget = target;
	}

	void PacNPC::SetFrightened(float /*duration*/)
	{
		std::cout << "AAAAAAA\n";
	}

	void dae::PacNPC::OnCollision(ICollider& other)
	{
		if (other.GetOwner()->GetTag() == PacData::PacTags::PacMan && m_Vulnerable)
		{
			OnNPCDeath.Invoke();
		}
	}

}
