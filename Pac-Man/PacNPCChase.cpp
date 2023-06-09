#include "PacNPCChase.h"
#include "PacNPC.h"
#include "PacNavigator.h"
#include <iostream>

dae::PacNPCChase::PacNPCChase()
{
}

void dae::PacNPCChase::OnArrive(const PacNPC& npc)
{
	std::cout << "Arrived Pog\n";
	if (!npc.GetNavigator()->HasPath())
		npc.GetNavigator()->SetPathToNode(npc.GetTarget()->GetTransform()->GetWorldPosition());
}