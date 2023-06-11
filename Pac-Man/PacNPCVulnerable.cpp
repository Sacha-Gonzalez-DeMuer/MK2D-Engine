#include "PacNPCVulnerable.h"
#include "RenderComponent.h"
#include "PacData.h"
#include "PacNPC.h"
#include "PacNavigator.h"
#include "PacNPCEyes.h"
#include "GameTime.h"
#include "ResourceManager.h"

dae::PacNPCVulnerable::PacNPCVulnerable(GameObject* npc_go, float duration)
	: PacNPCState(npc_go) , m_switchedDir(false), m_defaultTexturePath(""), m_timer(duration), m_duration(duration), m_toggleInvertTimer(0.0f)
{
	auto render_comp = npc_go->GetComponent<RenderComponent>();
	m_defaultTexturePath = render_comp->GetFilename();
	m_pVulnerableTexture = ResourceManager::Get().LoadTexture(PacData::PacFiles::VulnerableGhost);
	m_pInvertedTexture = ResourceManager::Get().LoadTexture(PacData::PacFiles::InvertedGhost);
	render_comp->SetTexture(m_pVulnerableTexture);
}


bool dae::PacNPCVulnerable::OnArrive(const PacNPC& npc)
{
	if(PacNPCState::OnArrive(npc)) return true;

	if (!m_switchedDir)
	{
		auto pNavigator = npc.GetNavigator();
		auto current_dir = pNavigator->GetCurrentDirection();
		auto opposite_dir = static_cast<int>(current_dir) * -1;
		pNavigator->SetCurrentDirection(static_cast<Direction>(opposite_dir));
		m_switchedDir = true;
		return true;
	}

	return false;
}

void dae::PacNPCVulnerable::UpdateState()
{
	if (m_timer > 0)
	{
		m_timer -= GameTime::Get().DeltaTime();
		if (m_timer < m_duration * .5f)
			m_toggleInvertTimer += GameTime::Get().DeltaTime();

		if (m_toggleInvertTimer > 0.2f)
		{
			m_toggleInvertTimer = 0;
			auto render_comp = m_pNPC->GetComponent<RenderComponent>();
			render_comp->SetTexture
			(render_comp->GetTexture() == m_pVulnerableTexture
				? m_pInvertedTexture : m_pVulnerableTexture);
		}
	}
	else 
	{
		OnExit();
		m_pNPC->GetComponent<PacNPC>()->ResetState(false);
	}
}

void dae::PacNPCVulnerable::OnExit()
{
	auto render_comp = m_pNPC->GetComponent<RenderComponent>();
	render_comp->SetTexture(m_defaultTexturePath);
}