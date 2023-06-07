#include "PacNPC.h"
#include "PacNavigator.h"
#include "PacNPCState.h"
#include "MathHelpers.h"
#include "PacData.h"

dae::PacNPC::PacNPC(std::shared_ptr<PacNavigator> pNavigator)
	: m_pNavigator{ pNavigator }, m_State{ PacNPCState::Wander }
{
	m_pNavigator->OnArriveAtTarget.AddFunction(std::bind(&PacNPC::OnArrive, this));
}

void dae::PacNPC::Update()
{
	// if distance between player and ghost is bigger than 10 tiles, chase
	// else wander
	const glm::vec2& targetPos = m_pTarget->GetTransform()->GetWorldPosition();
	const glm::vec2& npcPos = m_gameObject.lock()->GetTransform()->GetWorldPosition();
	const glm::vec2& dirToTarget = targetPos - npcPos;


	if (glm::length(dirToTarget) > 300)
		m_State = PacNPCState::Chase;


	if (!m_pNavigator->HasTarget())
		OnArrive();
}

void dae::PacNPC::SetTarget(std::shared_ptr<GameObject> target)
{
	m_pTarget = target;
}

void dae::PacNPC::OnArrive()
{
	switch (m_State)
	{
	case PacNPCState::Chase:
		ChaseLogic();
		break;

	case PacNPCState::Wander:
		WanderLogic();
		break;
	}
}

void dae::PacNPC::ChaseLogic()
{
	if(!m_pNavigator->HasPath()) 
		m_pNavigator->SetPathToNode(m_pTarget->GetTransform()->GetWorldPosition());

	//// Get the player's position
	//const glm::vec2& targetPos = m_pTarget->GetTransform()->GetWorldPosition();

	//// Get the NPC's position
	//const glm::vec2& npcPos = m_gameObject.lock()->GetTransform()->GetWorldPosition();

	//// Calculate the difference in position between the NPC and the player
	//const glm::vec2& dirToTarget = targetPos - npcPos;

	//// Calculate the absolute differences in position
	//glm::vec2 absPositionDiff = glm::abs(dirToTarget);


	//// Check if the desired direction is valid, and if not, select the next best direction
	//if (absPositionDiff.x > absPositionDiff.y)
	//{
	//	// Move horizontally
	//	if (dirToTarget.x > 0)
	//	{
	//		if (m_pNavigator->Move(Direction::RIGHT))
	//			return;
	//		else if (m_pNavigator->Move(Direction::LEFT))
	//			return;
	//	}
	//	else
	//	{
	//		if (m_pNavigator->Move(Direction::LEFT))
	//			return;
	//		else if (m_pNavigator->Move(Direction::RIGHT))
	//			return;
	//	}
	//}
	//else
	//{
	//	// Move vertically
	//	if (dirToTarget.y > 0)
	//	{
	//		if (m_pNavigator->Move(Direction::DOWN))
	//			return;
	//		else if (m_pNavigator->Move(Direction::UP))
	//			return;
	//	}
	//	else
	//	{
	//		if (m_pNavigator->Move(Direction::UP))
	//			return;
	//		else if (m_pNavigator->Move(Direction::DOWN))
	//			return;
	//	}
	//}
}

void dae::PacNPC::WanderLogic()
{
	int rndDir{ 0 };
	int currDir = static_cast<int>(m_pNavigator->GetCurrentDirection());

	while (rndDir == 0 || abs(rndDir) == abs(currDir))
		rndDir = dae::MathHelpers::GenerateRandomRange(-2, 2); //directions are mapped in this range

	m_pNavigator->Move(static_cast<Direction>(rndDir));
}
