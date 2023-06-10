#include "PacGameManager.h"
#include "PacGrid.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "AStarPathFinder.h"
#include "PacNavigator.h"
#include "PacNPC.h"
#include "PacLevel.h"
#include "PacData.h"
#include "GameObject.h"
#include "PacJSONParser.h"
#include "CircleCollider.h"
#include "PacScoreComponent.h"
#include "PacScoreHUD.h"
#include "PacHealthHUD.h"
#include "PacHealthComponent.h"
#include "PacController.h"
#include "PacNPCState.h"
#include "GameObject.h"
#include <iostream>
#include <memory>

dae::PacGameManager::PacGameManager(const PacData::PacGameData& gamedata)
	: m_GameData(gamedata)
{
}

void dae::PacGameManager::Start()
{
}

void dae::PacGameManager::LoadLevel(int idx, Scene& toScene)
{
	const auto& map = m_GameData.maps[idx];
	auto level_go = std::make_shared<GameObject>();

	auto level_grid = level_go->AddComponent<PacGrid>(map);

	PacData::PacLevelData levelData{};
	levelData.map = map;
	auto level = level_go->AddComponent<PacLevel>(levelData, level_grid);

	m_pCurrentLevel = level;

	auto pacman_go = std::make_shared<GameObject>();
	pacman_go->AddComponent<RenderComponent>()->SetTexture("pacman.png");

	auto astar_pathfinder = std::make_shared<AStarPathFinder>(level->GetGrid());

	auto ghost_go = std::make_shared<GameObject>();
	ghost_go->SetTag(PacData::PacTags::Ghost);
	ghost_go->AddComponent<RenderComponent>()->SetTexture("ghost.png");
	auto ghost_navigator = ghost_go->AddComponent<PacNavigator>(level->GetPacGrid(), astar_pathfinder);
	auto ghost_brain = ghost_go->AddComponent<PacNPC>(ghost_navigator);
	ghost_brain->SetTarget(pacman_go);
	ghost_go->AddComponent<CircleCollider>(static_cast<float>(level->GetGrid()->GetCellSize()));

	// Tell ghost what to do when he arrives at a node
	std::weak_ptr weak_brain = ghost_brain;
	ghost_navigator->OnArriveAtTarget.AddFunction([weak_brain](int, std::shared_ptr<PacGrid>) {
		if (auto ghost_brain_locked = weak_brain.lock()) {
			weak_brain.lock()->GetState()->OnArrive(*weak_brain.lock());
		}});


	auto pac_navigator = pacman_go->AddComponent<PacNavigator>(level->GetPacGrid(), astar_pathfinder);
	auto pac_controller = pacman_go->AddComponent<PacController>(pac_navigator);
	auto pac_score = pacman_go->AddComponent<PacScoreComponent>(0);
	pacman_go->AddComponent<CircleCollider>(static_cast<float>(level->GetGrid()->GetCellSize()));

	// Tell pacman what to do when he arrives at a node
	std::weak_ptr weak_score = pac_score;
	pac_navigator->OnArriveAtTarget.AddFunction([weak_score](int idx, std::shared_ptr<PacGrid> grid)
		{
			if (auto score = weak_score.lock(); score != nullptr)
			{
				// Collect Items
				const auto& info = grid->GetPacNodeInfo(idx);
				if (!info.hasItem) return;

				switch (info.type)
				{
				case PacData::PacNodeType::DOT:
					score->AddScore(PacData::PacDotScore);
					grid->SetPacNodeInfo(idx, PacData::PacNodeType::DOT, false);
					std::cout << "Collected dot!\n";
					break;

				case PacData::PacNodeType::POWERUP:
					score->AddScore(PacData::PacPowerUpScore);
					grid->SetPacNodeInfo(idx, PacData::PacNodeType::POWERUP, false);
					std::cout << "Collected POWERUP!\n";
					break;
				default:
					break;
				}
			}
		}, { weak_score });

	std::weak_ptr weak_controller = pac_controller;
	pac_navigator->OnArriveAtTarget.AddFunction([weak_controller](int idx, std::shared_ptr<PacGrid> grid)
		{
			if (auto controller = weak_controller.lock(); controller != nullptr)
			{
				const auto& info = grid->GetPacNodeInfo(idx);
				if (info.type == PacData::PacNodeType::POWERUP && info.hasItem)
					controller->PowerUp(5.f);
			}
		}, { weak_controller });



	const float margin = g_WindowSize.x * .02f;
	auto score_hud = std::make_shared<GameObject>();
	score_hud->AddComponent<PacScoreHUD>(pac_score);
	score_hud->GetTransform()->SetLocalPosition(margin, margin);

	auto pac_health = pacman_go->AddComponent<PacHealthComponent>(3);

	auto health_hud = std::make_shared<GameObject>();
	health_hud->AddComponent<PacHealthHUD>(pac_health);
	health_hud->GetTransform()->SetLocalPosition(margin, margin + 50.f);


	toScene.Add(level_go);
	toScene.Add(pacman_go);
	toScene.Add(ghost_go);
	toScene.Add(score_hud);
	toScene.Add(health_hud);
}
