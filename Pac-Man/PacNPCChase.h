#pragma once
#include "PacNPCState.h"


namespace dae
{
	class PacNPCChase final : public PacNPCState
	{
	public:
		PacNPCChase(PacNPC* pPacNPC);
		virtual ~PacNPCChase() = default;
		virtual void Update(PacNPC& npc) override;

	private:
		PacNPC* m_pPacNPC;
		int m_TargetNode;
	};
}
