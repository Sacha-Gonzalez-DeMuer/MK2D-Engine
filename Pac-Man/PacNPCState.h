#pragma once

namespace dae
{
	class PacNPC;
	class PacNPCState
	{
	public:
		virtual ~PacNPCState() = default;
		virtual bool OnArrive(const PacNPC& npc); // returns true if move behavior was updated
	};
}