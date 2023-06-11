#include "PacImpostorEyesState.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "PacNavigator.h"
#include "PacController.h"


dae::PacImpostorEyesState::PacImpostorEyesState(GameObject* npc_go)
	: PacNPCEyes(npc_go) {

	npc_go->GetComponent<PacController>()->SetGenericControllerActive(false);
}

//void dae::PacImpostorEyesState::UpdateState()
//{
//	if (m_pRenderComponent->GetTexture() != m_pEyesTexture)
//		m_pRenderComponent->SetTexture(m_pEyesTexture);
//
//	if (m_regenTimer > 0 && m_pNavigator->GetCurrentNodeIdx() == m_spawnNodeIdx)
//	{
//		m_regenTimer -= GameTime::Get().DeltaTime();
//	}
//	else if (m_regenTimer <= 0)
//	{
//		m_regenTimer = 5;
//	}
//}

void dae::PacImpostorEyesState::OnExit()
{
	PacNPCEyes::OnExit();
	m_pNPC->GetComponent<PacController>()->SetGenericControllerActive(true);
}
	