#pragma once
#include "PacNPCState.h"

namespace dae
{ 
	class PacNPCVulnerable final : public PacNPCState
	{
	public:
		virtual ~PacNPCVulnerable() = default;
		virtual bool OnArrive(const PacNPC& npc) override;

	};
}
