#pragma once
#include "PacNPC.h"
#include "PacNPCState.h"

namespace dae
{
	class PacNPCWander final : public PacNPCState
	{
	public:
		PacNPCWander() {};
		virtual ~PacNPCWander() = default;
		virtual bool OnArrive(const PacNPC& npc) override;
		
	private:
	};
}