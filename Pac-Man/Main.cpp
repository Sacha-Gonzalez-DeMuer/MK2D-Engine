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
#include "LevelComponent.h"
#include "PacController.h"
#include "GridNavComponent.h"
#include "AStarPathFinder.h"
#include "Commands.h"
#include "Input.h"
#include "SoundSystem.h"
#include "ServiceLocator.h"
#include "PacFilePaths.h"
#include <memory>
void LoadPacMan()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Pac-Man");

	auto map_go = std::make_shared<dae::GameObject>();
	auto level = map_go->AddComponent<dae::LevelComponent>(L"../Data/Level1.txt");

	auto pacman_go = std::make_shared<dae::GameObject>();
	pacman_go->AddComponent<dae::RenderComponent>()->SetTexture("pacman.png");

	auto astar_pathfinder = std::make_shared<dae::AStarPathFinder>(level->GetGrid());

	auto ghost_go = std::make_shared<dae::GameObject>();
	ghost_go->AddComponent<dae::RenderComponent>()->SetTexture("ghost.png");
	auto ghost_navigator = ghost_go->AddComponent<dae::GridNavComponent>(level->GetGrid(), astar_pathfinder);

	const float speed = 100.0f;
	auto pac_navigator = pacman_go->AddComponent<dae::GridNavComponent>(level->GetGrid(), astar_pathfinder, true);
	pacman_go->AddComponent<dae::PacController>(pac_navigator, speed);

	auto goto_command{ std::make_shared<dae::GoToCommand>(ghost_navigator, pac_navigator) };

	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_G, SDL_KEYDOWN), goto_command);

	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::SDL_SoundSystem>());
	dae::ServiceLocator::GetSoundSystem().AddSound(dae::FilePath::WAKAWAKA_SOUND, 0);
	dae::ServiceLocator::GetSoundSystem().PlaySound(0, 100, -1);

	scene.Add(map_go);
	scene.Add(pacman_go);
	scene.Add(ghost_go);
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(LoadPacMan);
	return 0;
}