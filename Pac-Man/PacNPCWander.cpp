#include "PacNPCWander.h"
#include "Datatypes.h"
#include "PacNPC.h"
#include "MathHelpers.h"
#include "PacNavigator.h"
#include <iostream>
void dae::PacNPCWander::OnArrive(const PacNPC& npc)
{
	std::cout << "Arrived pog\n";


	int rndDir{ 0 };
	int currDir = static_cast<int>(npc.GetNavigator()->GetCurrentDirection());

	const auto& legal_moves = npc.GetNavigator()->GetLegalMoves();

	if (legal_moves.empty()) return;

	if (legal_moves.size() >= 2)
	{
		do // find a random direction that is not the opposite of the current direction
		{
			rndDir = static_cast<int>(legal_moves[MathHelpers::GenerateRandomRange(0, static_cast<int>(legal_moves.size()) - 1)]);
		} while (rndDir == currDir * -1 || rndDir == 0);
	} 
	else rndDir = static_cast<int>(legal_moves[0]);

	npc.GetNavigator()->Move(static_cast<Direction>(rndDir));
}
