#include "PacNPCWander.h"
#include "Datatypes.h"
#include "PacNPC.h"
#include "MathHelpers.h"
#include "PacNavigator.h"

dae::PacNPCWander::PacNPCWander(PacNPC* pPacNPC)
	: m_pPacNPC{pPacNPC}
{
}

void dae::PacNPCWander::OnArrive(const PacNPC& npc)
{
	int rndDir{ 0 };
	int currDir = static_cast<int>(npc.GetNavigator()->GetCurrentDirection());

	while (rndDir == 0 || abs(rndDir) == abs(currDir))
		rndDir = dae::MathHelpers::GenerateRandomRange(-2, 2); //directions are mapped in this range

	npc.GetNavigator()->Move(static_cast<Direction>(rndDir));
}
