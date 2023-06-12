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


void dae::PacImpostorEyesState::OnExit()
{
	PacNPCEyes::OnExit();
	m_pNPC->GetComponent<PacController>()->SetGenericControllerActive(true);
}
	