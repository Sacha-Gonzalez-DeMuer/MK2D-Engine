#include "PacNPCVulnerable.h"


bool dae::PacNPCVulnerable::OnArrive(const PacNPC& npc)
{
	if(PacNPCState::OnArrive(npc)) return true;

	return false;
}
