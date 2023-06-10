#include "PacNPCState.h"
#include "PacNPC.h"
#include "PacNavigator.h"

bool dae::PacNPCState::OnArrive(const PacNPC& npc)
{
	if (npc.GetNavigator()->GetCurrentNodeInfo().type == PacData::PacNodeType::NPC_SPAWN)
	{
		npc.GetNavigator()->ExitSpawn();
		return true;
	}

	return false;
}
