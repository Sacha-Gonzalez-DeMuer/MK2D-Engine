#pragma once
#include "PacNPCState.h"

namespace dae
{
	class PacImpostorState final : public PacNPCState
	{
	public:
		PacImpostorState(GameObject* npc_go) : PacNPCState(npc_go) {};
		~PacImpostorState() = default;
		bool OnArrive(const PacNPC&) override { return false; };
	};
}