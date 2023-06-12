#include "PacNPC.h"
#include "PacNavigator.h"
#include "MathHelpers.h"
#include "PacData.h"
#include "ICollider.h"
#include "PacNPCState.h"
#include "PacNPCWander.h"
#include "PacNPCChase.h"
#include "PacNPCEyes.h"
#include "PacNPCVulnerable.h"
#include "GameTime.h"

namespace dae
{
	PacNPC::PacNPC(std::shared_ptr<PacNavigator> pNavigator)
		: m_pNavigator(pNavigator),
		m_State{std::make_shared<PacNPCWander>()}, m_DefaultState{ std::make_shared<PacNPCWander>() }
	{}

	void dae::PacNPC::Update()
	{
		m_State->UpdateState();

		if (m_pNavigator && !m_pNavigator->HasTarget() && m_State)
			m_State->OnArrive(*this);
	}

	void PacNPC::SetDefaultState(std::shared_ptr<PacNPCState> state, bool apply)
	{
		m_DefaultState = state;
		if (apply) m_State = m_DefaultState;
	}

	void PacNPC::SetState(std::shared_ptr<PacNPCState> state, bool onExit)
	{
		if(onExit) m_State->OnExit();
		m_State = state;
	}

	void PacNPC::ResetState(bool onExit)
	{
		SetState(m_DefaultState, onExit);
	}

	void dae::PacNPC::SetTarget(std::shared_ptr<GameObject> target)
	{
		m_pTarget = target;
	}

	void PacNPC::AddTarget(std::shared_ptr<GameObject> target)
	{
		m_pTargets.emplace_back(target);
	}

	void PacNPC::SetFrightened(float duration)
	{
		m_State = std::make_shared<PacNPCVulnerable>(GetOwner(), duration);
	}

	bool PacNPC::IsFrightened() const
	{
		return std::dynamic_pointer_cast<PacNPCVulnerable>(m_State) != nullptr;
	}

	void PacNPC::Die()
	{
		SetState(std::make_shared<PacNPCEyes>(GetOwner()));
		OnNPCDeath.Invoke();
	}

	void dae::PacNPC::OnCollisionEnter(ICollider& other)
	{
		m_State->HandleCollision(*this, other);
	}
}
