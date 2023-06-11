#include "PacImpostorVulnerableState.h"
#include "PacImpostorEyesState.h"
#include "PacNPC.h"
#include "ICollider.h"
#include "PacData.h"


void dae::PacImpostorVulnerableState::HandleCollision(PacNPC& npc, ICollider& other)
{
	if (other.GetOwner()->GetTag() == PacData::PacTags::PacMan)
	{
		npc.SetState(std::make_shared<PacImpostorEyesState>(npc.GetOwner()));
		npc.OnNPCDeath.Invoke();
	}
}

