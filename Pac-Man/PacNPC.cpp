#include "PacNPC.h"
#include "PacNavigator.h"
#include "PacNPCState.h"
#include "MathHelpers.h"
#include "PacData.h"

dae::PacNPC::PacNPC(std::shared_ptr<PacNavigator> pNavigator)
	: m_pNavigator{ pNavigator }, m_State{ nullptr }
{
	m_pNavigator->OnArriveAtTarget.AddFunction([this]() {
		m_State->OnArrive(*this);
		});
}

void dae::PacNPC::Update()
{
	if (!m_pNavigator->HasTarget())
		m_State->OnArrive(*this);
}

void dae::PacNPC::SetTarget(std::shared_ptr<GameObject> target)
{
	m_pTarget = target;
}
