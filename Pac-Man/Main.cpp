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
#include "PacGrid.h"

namespace dae
{
	void LoadPacMan()
	{
		auto& scene = SceneManager::GetInstance().CreateScene("Pac-Man");

		auto map_go = std::make_shared<GameObject>();
		auto level = map_go->AddComponent<PacLevel>(LevelData::PacLevels[0]);

		auto pacman_go = std::make_shared<GameObject>();
		pacman_go->AddComponent<RenderComponent>()->SetTexture("pacman.png");

		auto astar_pathfinder = std::make_shared<AStarPathFinder>(level->GetGrid());

		auto ghost_go = std::make_shared<GameObject>();
		ghost_go->AddComponent<RenderComponent>()->SetTexture("ghost.png");
		auto ghost_navigator = ghost_go->AddComponent<PacNavigator>(level->GetPacGrid(), astar_pathfinder);

		auto pac_navigator = pacman_go->AddComponent<PacNavigator>(level->GetPacGrid(), astar_pathfinder);
		pacman_go->AddComponent<PacController>(pac_navigator);

		auto spawnPos = level->GetPacGrid()->GetSpawnPos();
		pacman_go->GetTransform()->SetLocalPosition(spawnPos);


		//auto goto_command{ std::make_shared<GoToCommand>(ghost_navigator, pac_navigator) };
		//Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_G, SDL_KEYDOWN), goto_command);

		//ServiceLocator::RegisterSoundSystem(std::make_unique<SDL_SoundSystem>());
		//ServiceLocator::GetSoundSystem().AddSound(FilePath::WAKAWAKA_SOUND, 0);
		//ServiceLocator::GetSoundSystem().PlaySound(0, 100, -1);

		scene.Add(map_go);
		scene.Add(pacman_go);
		scene.Add(ghost_go);
	}

}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(dae::LoadPacMan);
	return 0;
}