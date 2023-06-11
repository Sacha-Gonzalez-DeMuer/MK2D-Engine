#pragma once
#include <memory>
namespace dae
{
	class PacNPC;
	class GameObject;
	class PacNPCState
	{
	public:
		PacNPCState(GameObject* npc_go) : m_pNPC{ npc_go } {};
		virtual ~PacNPCState() = default;
		virtual bool OnArrive(const PacNPC& npc); // returns true if move behavior was updated
		virtual void UpdateState() {};
		virtual void OnExit() {};
	protected:
		GameObject* m_pNPC;
	};
}