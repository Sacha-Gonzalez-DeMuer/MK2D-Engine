#pragma once
#include "PacNPCState.h"
#include <memory>

namespace dae
{
	class GameObject;
	class PacNPCChase final : public PacNPCState
	{
	public:
		PacNPCChase() : PacNPCState(nullptr) {};
		virtual ~PacNPCChase() = default;
		virtual bool OnArrive(const PacNPC& npc) override;
	private:
	};
}