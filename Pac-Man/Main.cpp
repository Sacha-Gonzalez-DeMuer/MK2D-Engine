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
#include "PacGameManager.h"

namespace dae
{
	void LoadPacMan()
	{
		auto& scene = SceneManager::Get().CreateScene("Pac-Man");

		auto jsonParser = std::make_shared<PacJSONParser>(PacData::PacJSONpath);
		PacData::PacGameData game_data{};
		jsonParser->LoadGameData(game_data);

		auto game_manager_go = std::make_shared<GameObject>();
		auto game_manager = game_manager_go->AddComponent<PacGameManager>(game_data);
		scene.Add(game_manager_go);

		game_manager->LoadLevel(2, scene);

		SceneManager::Get().SetActiveScene("Pac-Man");
	}
}


int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(dae::LoadPacMan);
	return 0;
}