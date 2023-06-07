#include "PacNPC.h"
#include "PacNavigator.h"
#include "PacNPCState.h"

dae::PacNPC::PacNPC(std::shared_ptr<PacNavigator> pNavigator)
	: m_pNavigator{ pNavigator }
{}

void dae::PacNPC::Update()
{
	m_pState->Update(*this);
}
