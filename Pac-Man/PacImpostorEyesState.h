#pragma once
#include "PacNPCEyes.h"

namespace dae
{
	class PacImpostorEyesState final : public PacNPCEyes
	{
	public:
		PacImpostorEyesState(GameObject* npc_go);
		~PacImpostorEyesState() = default;
		//virtual void UpdateState() override;
		virtual void OnExit() override;
	};
}
