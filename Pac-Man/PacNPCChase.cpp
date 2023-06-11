#include "PacNPCChase.h"
#include "PacNPC.h"
#include "PacNavigator.h"
#include <iostream>


bool dae::PacNPCChase::OnArrive(const PacNPC& npc)
{
	if (PacNPCState::OnArrive(npc)) return true;

	//ideally wouldve only had to be called once but something goes wrong with the direction queue & no time to fix
	npc.GetNavigator()->SetPathToNode(npc.GetTarget()->GetTransform()->GetWorldPosition());
	return true;
}