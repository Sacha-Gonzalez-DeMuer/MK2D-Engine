#pragma once


namespace dae
{
	class PacNPC;
	class PacNPCWander;
	class PacNPCState
	{
	public:
		static PacNPCWander wander;

		virtual ~PacNPCState() = default;
		virtual void Update(PacNPC& /*npc*/) {};
	};
}
