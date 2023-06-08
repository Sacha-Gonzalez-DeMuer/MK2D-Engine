#pragma once
#include "PacNPC.h"
#include "PacNPCState.h"

namespace dae
{
	class PacNPCWander final : public PacNPCState
	{
	public:
		PacNPCWander(PacNPC* pPacNPC);
		PacNPCWander() = delete;
		virtual ~PacNPCWander() = default;
		virtual void OnArrive(const PacNPC& npc) override;
		
	private:
		PacNPC* m_pPacNPC;
	};
}
