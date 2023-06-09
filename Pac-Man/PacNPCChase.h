#pragma once
#include "PacNPCState.h"
#include <memory>

namespace dae
{
	class GameObject;
	class PacNPCChase final : public PacNPCState
	{
	public:
		PacNPCChase();
		virtual ~PacNPCChase() = default;
		virtual void OnArrive(const PacNPC& npc) override;

	private:
	};
}