#pragma once
#include "PacNPCVulnerable.h"

namespace dae
{
	class PacNPC;
	class PacImpostorVulnerableState final : public PacNPCVulnerable
	{
	public:
		PacImpostorVulnerableState(GameObject* npc_go, float duration) : PacNPCVulnerable(npc_go, duration) {};
		~PacImpostorVulnerableState() = default;

		virtual bool OnArrive(const PacNPC&) override { return false; };
		virtual void HandleCollision(PacNPC& npc, ICollider& other) override;
	};
}
