#include <SDL.h>

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "PacController.h"
#include "AStarPathFinder.h"
#include "Commands.h"
#include "Input.h"
#include "SoundSystem.h"
#include "ServiceLocator.h"
#include "PacFilePaths.h"
#include <memory>
#include "PacLevel.h"
#include "PacData.h"
#include "PacNavigator.h"
#include "PacNPCState.h"
#include "PacGrid.h"
#include "PacNPC.h"
#include "PacJSONParser.h"
#include "CircleCollider.h"
#include "PacScoreComponent.h"
#include <iostream>
#include "PacScoreHUD.h"
#include "PacHealthHUD.h"
#include "PacHealthComponent.h"

namespace dae
{
	void LoadPacMan()
	{
		auto& scene = SceneManager::GetInstance().CreateScene("Pac-Man");

		auto jsonParser = std::make_shared<PacJSONParser>(PacData::PacJSONpath);
		PacData::PacLevelData level0{};
		jsonParser->LoadLevel(0, level0);

		auto level_go = std::make_shared<GameObject>();
		auto level_grid = level_go->AddComponent<PacGrid>(level0.map);
		auto level = level_go->AddComponent<PacLevel>(level0, level_grid);
		level_go->GetTransform()->SetLocalPosition(200, 200);
		
		auto pacman_go = std::make_shared<GameObject>();
		pacman_go->AddComponent<RenderComponent>()->SetTexture("pacman.png");
		
		auto astar_pathfinder = std::make_shared<AStarPathFinder>(level->GetGrid());
		
		auto ghost_go = std::make_shared<GameObject>();
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
		pac_controller->OnPlayerDeath.AddFunction([](){std::cout << "Player died\n";});
		auto pac_score = pacman_go->AddComponent<PacScoreComponent>(0);
		
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


		auto score_hud = std::make_shared<GameObject>();
		score_hud->AddComponent<PacScoreHUD>(pac_score);
	  
		auto pac_health = pacman_go->AddComponent<PacHealthComponent>(3);

		auto health_hud = std::make_shared<GameObject>();
		health_hud->AddComponent<PacHealthHUD>(pac_health);
		




		//pacman_go->AddComponent<CircleCollider>(static_cast<float>(level->GetGrid()->GetCellSize()));
		//
		//auto spawnPos = level->GetPacGrid()->GetSpawnPos();
		//pacman_go->GetTransform()->SetLocalPosition(spawnPos);
		//
		


		//auto goto_command{ std::make_shared<GoToCommand>(ghost_navigator, pac_navigator) };
		//Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_G, SDL_KEYDOWN), goto_command);

		//ServiceLocator::RegisterSoundSystem(std::make_unique<SDL_SoundSystem>());
		//ServiceLocator::GetSoundSystem().AddSound(FilePath::WAKAWAKA_SOUND, 0);
		//ServiceLocator::GetSoundSystem().PlaySound(0, 100, -1);

		scene.Add(level_go);
		scene.Add(pacman_go);
		scene.Add(ghost_go);
		scene.Add(score_hud);
		scene.Add(health_hud);
	}

}


class ObjectToDestroy
{
public:
	ObjectToDestroy() { std::cout << "ObjectToDestroy constructed\n"; }
	~ObjectToDestroy() { std::cout << "ObjectToDestroy destroyed\n"; }
};

class TestClass
{
public:
	void TestFunction() { std::cout << "TestFunction called\n"; }
};




int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(dae::LoadPacMan);
	return 0;
}