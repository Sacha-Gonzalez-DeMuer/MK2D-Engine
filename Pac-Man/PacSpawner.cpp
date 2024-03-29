#include "PacSpawner.h"
#include "PacNPC.h"
#include "PacNavigator.h"
#include "PacLevel.h"
#include "PacData.h"
#include "PacLevel.h"
#include "AStarPathFinder.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "CircleCollider.h"
#include "GridGraph.h"
#include "PacNPCState.h"
#include "PacGrid.h"
#include "MathHelpers.h"

#include "PacNPCChase.h"

namespace dae
{
	PacSpawner::PacSpawner(std::shared_ptr<PacLevel> level)
		: m_pNPCs{ 4 }, m_pLevel{ level }, m_ghostSize{static_cast<float>(level->GetGrid()->GetCellSize())}, m_spawnIndices{ level->GetPacGrid()->GetNPCSpawnIdxs() }
	{}

	void PacSpawner::Initialize()
	{
		m_pNPCs[static_cast<int>(PacData::PacGhosts::BLINKY)] = InitializeBlinky();
		m_pNPCs[static_cast<int>(PacData::PacGhosts::PINKY)] = InitializePinky();
		m_pNPCs[static_cast<int>(PacData::PacGhosts::INKY)] = InitializeInky();
		m_pNPCs[static_cast<int>(PacData::PacGhosts::CLYDE)] = InitializeClyde();

		for (auto& npc : m_pNPCs)
			GetOwner()->AddChild(npc);
	}

	std::shared_ptr<GameObject> PacSpawner::InitializeGenericNPC()
	{
		auto ghost_go = std::make_shared<GameObject>();
		auto astar_pathfinder = std::make_shared<AStarPathFinder>(m_pLevel->GetGrid());
		auto ghost_navigator = ghost_go->AddComponent<PacNavigator>(m_pLevel->GetPacGrid(), astar_pathfinder);
		auto ghost_brain = ghost_go->AddComponent<PacNPC>(ghost_navigator);

		ghost_go->SetTag(PacData::PacTags::Ghost);
		ghost_go->AddComponent<CircleCollider>(static_cast<float>(m_ghostSize * .5f));
		ghost_navigator->SetCurrentNode(m_spawnIndices[
			MathHelpers::GenerateRandomRange(0, static_cast<int>(m_spawnIndices.size()-1))]);

		// Blinky behavior
		std::weak_ptr weak_brain = ghost_brain;
		ghost_navigator->OnArriveAtTarget.AddFunction([weak_brain](int, std::shared_ptr<PacGrid>) {

			if (auto ghost_brain_locked = weak_brain.lock()) {
				weak_brain.lock()->GetState()->OnArrive(*weak_brain.lock());
			}});

		return ghost_go;
	}

	std::shared_ptr<GameObject> PacSpawner::InitializeBlinky()
	{
		auto blinky = InitializeGenericNPC();
		auto blinkys_brain = blinky->GetComponent<PacNPC>();
		auto ghost_navigator = blinky->GetComponent<PacNavigator>();
		auto blinky_img = blinky->AddComponent<RenderComponent>();

		blinkys_brain->SetDefaultState(std::make_shared<PacNPCChase>());
		blinky_img->SetTexture(PacData::PacFiles::Blinky);
		blinky_img->SetSize({ m_ghostSize, m_ghostSize });



		return blinky;
	}

	std::shared_ptr<GameObject> PacSpawner::InitializePinky()
	{
		auto pinky = InitializeGenericNPC();
		auto pinkys_brain = pinky->GetComponent<PacNPC>();
		auto ghost_navigator = pinky->GetComponent<PacNavigator>();
		auto pinky_img = pinky->AddComponent<RenderComponent>();

		pinky_img->SetTexture(PacData::PacFiles::Pinky);
		pinky_img->SetSize({ m_ghostSize, m_ghostSize });


		return pinky;
	}

	std::shared_ptr<GameObject> PacSpawner::InitializeInky()
	{
		auto inky = InitializeGenericNPC();
		auto inkys_brain = inky->GetComponent<PacNPC>();
		auto ghost_navigator = inky->GetComponent<PacNavigator>();
		auto inky_img = inky->AddComponent<RenderComponent>();

		inky_img->SetTexture(PacData::PacFiles::Inky);
		inky_img->SetSize({ m_ghostSize, m_ghostSize });

		return inky;
	}

	std::shared_ptr<GameObject> PacSpawner::InitializeClyde()
	{
		auto clyde = InitializeGenericNPC();
		auto clydes_brain = clyde->GetComponent<PacNPC>();
		auto ghost_navigator = clyde->GetComponent<PacNavigator>();
		auto clyde_img = clyde->AddComponent<RenderComponent>();

		clyde_img->SetTexture(PacData::PacFiles::Clyde);
		clyde_img->SetSize({ m_ghostSize, m_ghostSize });


		return clyde;
	}

}
