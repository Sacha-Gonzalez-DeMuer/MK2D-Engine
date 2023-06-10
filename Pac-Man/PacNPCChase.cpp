#include "PacNPCChase.h"
#include "PacNPC.h"
#include "PacNavigator.h"
#include <iostream>

dae::PacNPCChase::PacNPCChase()
{
}

bool dae::PacNPCChase::OnArrive(const PacNPC& npc)
{
	if (PacNPCState::OnArrive(npc)) return true;

	if (!npc.GetNavigator()->HasPath())
	{
		npc.GetNavigator()->SetPathToNode(npc.GetTarget()->GetTransform()->GetWorldPosition());
		return true;
	}

	return false;
}