#pragma once
#include "PacNPCState.h"
#include "Texture2D.h"
#include <string>

namespace dae
{ 
	class GameObject;
	class RenderComponent;
	class PacNPCVulnerable final : public PacNPCState
	{
	public:
		PacNPCVulnerable(GameObject* npc_go, float duration);
		virtual ~PacNPCVulnerable() = default;
		virtual bool OnArrive(const PacNPC& npc) override;
		virtual void OnExit() override;
		virtual void UpdateState() override;

	private:
		std::string m_defaultTexturePath;
		std::shared_ptr<Texture2D> m_pVulnerableTexture;
		std::shared_ptr<Texture2D> m_pInvertedTexture;
		bool m_switchedDir;
		float m_duration;
		float m_timer;
		float m_toggleInvertTimer;
	};
}
