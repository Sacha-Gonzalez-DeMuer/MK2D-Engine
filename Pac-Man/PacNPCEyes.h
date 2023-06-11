#pragma once
#include "PacNPCState.h"
#include <string>
namespace dae
{
	class PacNavigator;
	class Texture2D;
	class RenderComponent;
	class PacNPCEyes final : public PacNPCState
	{
	public:
		PacNPCEyes(GameObject* npc_go);
		virtual ~PacNPCEyes() = default;
		virtual void OnExit() override;
		virtual void UpdateState() override;
		virtual bool OnArrive(const PacNPC& npc) override;

	private:
		std::string m_defaultTexturePath;
		std::shared_ptr<RenderComponent> m_pRenderComponent;
		std::shared_ptr<Texture2D> m_pEyesTexture;
		std::shared_ptr<PacNavigator> m_pNavigator;
		int m_spawnNodeIdx;
		float m_regenTimer;
	};
}

