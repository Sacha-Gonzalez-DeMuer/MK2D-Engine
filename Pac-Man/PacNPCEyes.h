#pragma once
#include "PacNPCState.h"
#include <string>
namespace dae
{
	class PacNavigator;
	class Texture2D;
	class RenderComponent;
	class PacNPCEyes : public PacNPCState
	{
	public:
		PacNPCEyes(GameObject* npc_go);
		virtual ~PacNPCEyes() = default;
		virtual void OnExit() override;
		virtual void UpdateState() override;
		virtual bool OnArrive(const PacNPC& npc) override;

	protected:
		std::shared_ptr<RenderComponent> m_pRenderComponent;
		std::shared_ptr<Texture2D> m_pEyesTexture;
		std::shared_ptr<PacNavigator> m_pNavigator;
		std::string m_defaultTexturePath;
		int m_spawnNodeIdx;
		float m_regenTimer;
	};
}

