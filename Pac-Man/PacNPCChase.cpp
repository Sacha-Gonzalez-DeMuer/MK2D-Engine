#include "PacNPCChase.h"
#include "PacNPC.h"
#include "PacNavigator.h"
dae::PacNPCChase::PacNPCChase(PacNPC* pPacNPC)
	: m_pPacNPC{pPacNPC}
{
}

void dae::PacNPCChase::OnArrive(const PacNPC& npc)
{
	if (!npc.GetNavigator()->HasPath())
		npc.GetNavigator()->SetPathToNode(npc.GetTarget()->GetTransform()->GetWorldPosition());
}
