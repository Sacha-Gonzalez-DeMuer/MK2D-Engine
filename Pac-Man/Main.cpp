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
#include "PlayerController.h"
#include "GridNavComponent.h"
#include "AStarPathFinder.h"
#include "Heuristic.h"	
#include <memory>
#include "Commands.h"
#include "Input.h"

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

	auto pac_navigator = pacman_go->AddComponent<dae::GridNavComponent>(level->GetGrid(), astar_pathfinder);
	pacman_go->AddComponent<dae::PlayerController>(pac_navigator);

	auto goto_command{ std::make_shared<dae::GoToCommand>(ghost_navigator, pac_navigator) };

	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_G, SDL_KEYDOWN), goto_command);

	scene.Add(map_go);
	scene.Add(pacman_go);
	scene.Add(ghost_go);
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(LoadPacMan);
	return 0;
}

/* #include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include "Input.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "FPSCounter.h"
#include "Orbiter.h"
#include "CacheTrasher.h"
#include "CacheTrashDebugger.h"
#include "HealthComponent.h"
#include "HealthHUD.h"
#include "ScoreComponent.h"
#include "ScoreHUD.h"
#include "CircleCollider.h"
#include "Datatypes.h"

//pacman
#include "LevelComponent.h"
#include "PlayerController.h"
#include "CGraphNodeNavigator.h"
#include "AStarPathfinder.h"

#include <chrono>
#include <iostream>



void AddFPSCounter(dae::Scene& scene, std::shared_ptr<dae::Font> font);
void AddOrbitSystem(dae::Scene& scene);
void AddCacheTrasher(dae::Scene& scene);
void AddInputAssignment(dae::Scene& scene);
void AddObserverAssignment(dae::Scene& scene, std::shared_ptr<dae::Font> font);

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	// background
	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>()->SetTexture("background.tga");
	scene.Add(go);

	// logo
	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>()->SetTexture("logo.tga");
	go->SetLocalPosition(216, 180);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);

	auto go_assignment = std::make_shared<dae::GameObject>();
	go_assignment->SetLocalPosition(80, 20);
	go_assignment->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	scene.Add(go_assignment);

	// FPS counter
	AddFPSCounter(scene, font);

	//AddInputAssignment(scene);

	AddObserverAssignment(scene, font);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(LoadPacMan);
	return 0;
}

void AddFPSCounter(dae::Scene& scene, std::shared_ptr<dae::Font> font)
{
	auto go_fps_counter = std::make_shared<dae::GameObject>();
	go_fps_counter->AddComponent<dae::FPSCounter>();
	go_fps_counter->AddComponent<dae::TextComponent>("FPS", font);
	go_fps_counter->SetLocalPosition(0, 0);
	scene.Add(go_fps_counter);
}

void AddOrbitSystem(dae::Scene& scene)
{
	// === Scenegraph objects
	// create sun
	auto go_sun = std::make_shared<dae::GameObject>();
	go_sun->AddComponent<dae::RenderComponent>()->SetTexture("sun.png");
	go_sun->SetLocalPosition(250, 250);

	// create earth
	auto go_earth = std::make_shared<dae::GameObject>();
	go_earth->AddComponent<dae::RenderComponent>()->SetTexture("earth.png");
	go_earth->AddComponent<dae::Orbiter>(150.0f);

	// create moon
	auto go_moon = std::make_shared<dae::GameObject>();
	go_moon->AddComponent<dae::RenderComponent>()->SetTexture("moon.png");
	go_moon->AddComponent<dae::Orbiter>(100.0f);

	// create hierarchy
	//go_moon->SetParent(go_earth, false);
	go_sun->AddChild(go_earth);

	// add celestial bodies to scene
	scene.Add(go_sun);
}

void AddCacheTrasher(dae::Scene& scene)
{
	auto cache_trasher_go{ std::make_shared<dae::GameObject>() };
	cache_trasher_go->AddComponent<dae::CacheTrashDebugger>();
	scene.Add(cache_trasher_go);
}

void AddInputAssignment(dae::Scene& scene)
{
	// set up objects
	auto controller0{ std::make_shared<dae::XBox360Controller>(0) };
	auto pacman{ std::make_shared<dae::GameObject>() };
	auto pacwoman{ std::make_shared<dae::GameObject>() };
	pacman->AddComponent<dae::RenderComponent>()->SetTexture("pacman.png");
	pacwoman->AddComponent<dae::RenderComponent>()->SetTexture("pacwoman.png");
	float speed{ 10.0f };

	// pacman commands
	auto move_right_command{ std::make_shared<dae::MoveCommand>(pacman, glm::vec2(1,0), speed) };
	auto move_left_command{ std::make_shared<dae::MoveCommand>(pacman, glm::vec2(-1,0), speed) };
	auto move_up_command{ std::make_shared<dae::MoveCommand>(pacman, glm::vec2(0,-1), speed) };
	auto move_down_command{ std::make_shared<dae::MoveCommand>(pacman, glm::vec2(0,1), speed) };

	// Controller binds
	dae::Input::GetInstance().AddController(controller0);
	dae::Input::GetInstance().AddCommand(std::make_pair(controller0->GetControllerIndex(), dae::XBox360Controller::ControllerButton::DPadRight), move_right_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(controller0->GetControllerIndex(), dae::XBox360Controller::ControllerButton::DPadLeft), move_left_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(controller0->GetControllerIndex(), dae::XBox360Controller::ControllerButton::DPadUp), move_up_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(controller0->GetControllerIndex(), dae::XBox360Controller::ControllerButton::DPadDown), move_down_command);

	// pacwoman commands
	speed *= 2;
	move_right_command = std::make_shared<dae::MoveCommand>(pacwoman, glm::vec2(1, 0), speed);
	move_left_command = std::make_shared<dae::MoveCommand>(pacwoman, glm::vec2(-1, 0), speed);
	move_up_command = std::make_shared<dae::MoveCommand>(pacwoman, glm::vec2(0, -1), speed);
	move_down_command = std::make_shared<dae::MoveCommand>(pacwoman, glm::vec2(0, 1), speed);

	// keyboard binds
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_D, SDL_KEYDOWN), move_right_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_W, SDL_KEYDOWN), move_up_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_A, SDL_KEYDOWN), move_left_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_S, SDL_KEYDOWN), move_down_command);

	scene.Add(pacman);
	scene.Add(pacwoman);
}


void AddObserverAssignment(dae::Scene& scene, std::shared_ptr<dae::Font> font)
{
	// === Player 1 === //
	auto player1{ std::make_shared<dae::GameObject>() };
	player1->AddComponent<dae::RenderComponent>()->SetTexture("pacman.png");
	player1->AddComponent<dae::CircleCollider>(10.0f);
	player1->SetLocalPosition(250, 250);
	player1->SetTag("Player");
	scene.Add(player1);

	// input
	float speed{ 3.0f };
	auto move_right_command{ std::make_shared<dae::MoveCommand>(player1, glm::vec2(1,0), speed) };
	auto move_left_command{ std::make_shared<dae::MoveCommand>(player1, glm::vec2(-1,0), speed) };
	auto move_up_command{ std::make_shared<dae::MoveCommand>(player1, glm::vec2(0,-1), speed) };
	auto move_down_command{ std::make_shared<dae::MoveCommand>(player1, glm::vec2(0,1), speed) };
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_W, SDL_KEYDOWN), move_up_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_A, SDL_KEYDOWN), move_left_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_S, SDL_KEYDOWN), move_down_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_D, SDL_KEYDOWN), move_right_command);

	// hud
	auto p1HUD{std::make_shared<dae::GameObject>()};
	auto healthComponent = player1->AddComponent<dae::HealthComponent>( 3);
	auto scoreComponent = player1->AddComponent<dae::ScoreComponent>(0);

	// health
	auto p1LivesTxtObj{std::make_shared<dae::GameObject>()};
	auto p1LivesTxtComponent = p1LivesTxtObj->AddComponent<dae::TextComponent>("Lives: " + std::to_string(healthComponent->GetRemainingLives()), font);

	auto p1HealthHUD{ std::make_shared<dae::GameObject>() };
	p1HealthHUD->AddComponent<dae::HealthHUD>(healthComponent, p1LivesTxtComponent);
	p1HealthHUD->AddChild(p1LivesTxtObj);
	scene.Add(p1LivesTxtObj);
	scene.Add(p1HealthHUD);

	// score
	auto p1ScoreTxtObj{ std::make_shared<dae::GameObject>() };
	auto p1ScoreTxtComponent = p1ScoreTxtObj->AddComponent<dae::TextComponent>("Score: " + std::to_string(scoreComponent->GetScore()), font);
	auto p1ScoreHUD{ std::make_shared<dae::GameObject>() };
	p1ScoreHUD->AddComponent<dae::ScoreHUD>(scoreComponent, p1ScoreTxtComponent);
	p1ScoreHUD->AddChild(p1ScoreTxtObj);
	p1ScoreHUD->SetLocalPosition(0.0f, 20.0f);
	scene.Add(p1ScoreHUD);
	scene.Add(p1ScoreTxtObj);


	p1HUD->AddChild(p1HealthHUD);
	p1HUD->AddChild(p1ScoreHUD);
	p1HUD->SetLocalPosition(0.0f, 50.0f);



	// === Player 2 === //
	auto player2{ std::make_shared<dae::GameObject>() };
	player2->AddComponent<dae::RenderComponent>()->SetTexture("pacwoman.png");
	player2->AddComponent<dae::CircleCollider>(10.0f);
	player2->SetTag("Player");
	scene.Add(player2);

	// input
	move_right_command = std::make_shared<dae::MoveCommand>(player2, glm::vec2(1,0), speed);
	move_left_command = std::make_shared<dae::MoveCommand>(player2, glm::vec2(-1,0), speed);
	move_up_command = std::make_shared<dae::MoveCommand>(player2, glm::vec2(0,-1), speed);
	move_down_command = std::make_shared<dae::MoveCommand>(player2, glm::vec2(0,1), speed);
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_I, SDL_KEYDOWN), move_up_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_J, SDL_KEYDOWN), move_left_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_K, SDL_KEYDOWN), move_down_command);
	dae::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_L, SDL_KEYDOWN), move_right_command);

	auto p2HUD{ std::make_shared<dae::GameObject>() };
	healthComponent = player2->AddComponent<dae::HealthComponent>(3);
	scoreComponent = player2->AddComponent<dae::ScoreComponent>(0);

	// health
	auto p2LivesTxtObj{ std::make_shared<dae::GameObject>() };
	auto p2LivesTxtComponent = p2LivesTxtObj->AddComponent<dae::TextComponent>("Lives: " + std::to_string(healthComponent->GetRemainingLives()), font);

	auto p2HealthHUD{ std::make_shared<dae::GameObject>() };
	p2HealthHUD->AddComponent<dae::HealthHUD>(healthComponent, p2LivesTxtComponent);
	p2HealthHUD->AddChild(p2LivesTxtObj);
	scene.Add(p2LivesTxtObj);
	scene.Add(p2HealthHUD);

	// score
	auto p2ScoreTxtObj{ std::make_shared<dae::GameObject>() };
	auto p2ScoreTxtComponent = p2ScoreTxtObj->AddComponent<dae::TextComponent>("Score: " + std::to_string(scoreComponent->GetScore()), font);
	auto p2ScoreHUD{ std::make_shared<dae::GameObject>() };
	p2ScoreHUD->AddComponent<dae::ScoreHUD>(scoreComponent, p2ScoreTxtComponent);
	p2ScoreHUD->AddChild(p2ScoreTxtObj);
	p2ScoreHUD->SetLocalPosition(0.0f, 20.0f);
	scene.Add(p2ScoreHUD);
	scene.Add(p2ScoreTxtObj);

	p2HUD->AddChild(p2HealthHUD);
	p2HUD->AddChild(p2ScoreHUD);
	p2HUD->SetLocalPosition(0.0f, 100.0f);


	// === Pac-Dots === //
	const int nrDots{ 100 };
	for (int i{}; i < nrDots; ++i)
	{
		auto dot{ std::make_shared<dae::GameObject>() };
		dot->AddComponent<dae::RenderComponent>()->SetTexture("dot.png");
		dot->AddComponent<dae::CircleCollider>(5.0f);
		dot->SetTag("Dot");
		dot->SetLocalPosition(float(rand() % 640), float(rand() % 480));
		scene.Add(dot);
	}


	// === Ghosts === // (they're lazy for now) :)
	const int nrGhosts{ 5 };
	for (int i{}; i < nrGhosts; ++i)
	{
		auto ghost{ std::make_shared<dae::GameObject>() };
		ghost->AddComponent<dae::RenderComponent>()->SetTexture("ghost.png");
		ghost->AddComponent<dae::CircleCollider>(10.0f);
		ghost->SetTag("Ghost");
		ghost->SetLocalPosition(float(rand() % 640), float(rand() % 480));
		scene.Add(ghost);
	}



}

*/