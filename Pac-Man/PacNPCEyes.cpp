#include "PacNPCEyes.h"
#include "PacData.h"
#include "RenderComponent.h"
#include "PacNavigator.h"
#include "PacGrid.h"
#include "PacNPC.h"
#include "GameTime.h"
#include "GameObject.h"

dae::PacNPCEyes::PacNPCEyes(GameObject* npc_go)
	: PacNPCState(npc_go), m_regenTimer{5}
{
	m_pRenderComponent = npc_go->GetComponent<RenderComponent>();
	m_defaultTexturePath = m_pRenderComponent->GetFilename();
	m_pEyesTexture = ResourceManager::Get().LoadTexture(PacData::PacFiles::NPCEyes);
	m_pNavigator = npc_go->GetComponent<PacNavigator>();
	int spawn_node_idx = m_pNavigator->GetPacGrid()->GetNPCSpawnIdxs()[0];
	m_pNavigator->SetPathToNode(spawn_node_idx);
	m_spawnNodeIdx = spawn_node_idx;
}

void dae::PacNPCEyes::OnExit()
{
	auto render_comp = m_pNPC->GetComponent<RenderComponent>();
	if (render_comp) render_comp->SetTexture(m_defaultTexturePath);
}

void dae::PacNPCEyes::UpdateState()
{
	if (m_pRenderComponent->GetTexture() != m_pEyesTexture)
		m_pRenderComponent->SetTexture(m_pEyesTexture);

	if (m_regenTimer > 0 && m_pNavigator->GetCurrentNodeIdx() == m_spawnNodeIdx)
	{
		m_regenTimer -= GameTime::Get().DeltaTime();
	}
	else if (m_regenTimer <= 0)
	{
		m_regenTimer = 5;
		auto npc = m_pNPC->GetComponent<PacNPC>();
		npc->ResetState();
	}
}

bool dae::PacNPCEyes::OnArrive(const PacNPC&)
{
	//ideally wouldve only had to be called once but something goes wrong with the direction queue & no time to fix
	m_pNavigator->SetPathToNode(m_spawnNodeIdx);
	return true;
}
