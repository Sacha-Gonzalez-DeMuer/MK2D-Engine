#pragma once

namespace dae
{
	class PacNPC;
	class PacNPCState
	{
	public:
		virtual ~PacNPCState() = default;
		virtual void OnArrive(const PacNPC& npc); 
	};
}