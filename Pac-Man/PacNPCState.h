#pragma once

namespace dae
{
	class PacNPC;
	class PacNPCWander;
	class PacNPCChase;
	class PacNPCState
	{
	public:
		static PacNPCWander wander;

		virtual ~PacNPCState() = default;
		virtual void OnArrive(const PacNPC&) {}; 
	};
}