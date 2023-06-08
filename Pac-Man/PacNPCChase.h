#pragma once
#include "PacNPCState.h"
#include <memory>

namespace dae
{
	class GameObject;
	class PacNPCChase final : public PacNPCState
	{
	public:
		PacNPCChase(PacNPC* pPacNPC);
		virtual ~PacNPCChase() = default;
		virtual void OnArrive(const PacNPC& npc) override;

	private:
		PacNPC* m_pPacNPC;
	};
}