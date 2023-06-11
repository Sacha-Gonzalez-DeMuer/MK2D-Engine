#include "PacNPCState.h"
#include "PacNPC.h"
#include "PacNavigator.h"
#include "ICollider.h"
#include "PacHealthComponent.h"

bool dae::PacNPCState::OnArrive(const PacNPC& npc)
{
	if (npc.GetNavigator()->GetCurrentNodeInfo().type == PacData::PacNodeType::NPC_SPAWN)
	{
		npc.GetNavigator()->ExitSpawn();
		return true;
	}

	return false;
}

void dae::PacNPCState::HandleCollision(PacNPC&, ICollider& other)
{
	if (other.GetOwner()->GetTag() == PacData::PacTags::PacMan)
	{
		auto pac_health = other.GetOwner()->GetComponent<PacHealthComponent>();
		pac_health->TakeHit();
	}
}

