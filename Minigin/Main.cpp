#include <SDL.h>

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
#include "InputManager.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "FPSCounter.h"
#include "Orbiter.h"
#include <chrono>


void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	auto renderComponent{ std::make_shared<dae::RenderComponent>() };
	renderComponent->SetTexture("background.tga");
	go->AddComponent(renderComponent);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	renderComponent = std::make_shared<dae::RenderComponent>();
	renderComponent->SetTexture("logo.tga");
	go->AddComponent(renderComponent);
	go->SetPosition(216, 180);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_shared<dae::TextObject>("Programming 4 Assignment", font);
	
	auto go_assignment = std::make_shared<dae::GameObject>();
	go_assignment->SetPosition(80, 20);
	go_assignment->AddComponent(to);
	scene.Add(go_assignment);

	auto go_fps_counter = std::make_shared<dae::GameObject>();
	auto fps_counter{ std::make_shared<dae::FPSCounter>() };
	to = std::make_shared<dae::TextObject>("FPS", font);
	go_fps_counter->AddComponent(to);
	go_fps_counter->AddComponent(fps_counter);
	go_fps_counter->SetPosition(0, 0);
	scene.Add(go_fps_counter);

	// === Scenegraph objects
	// create sun
	auto go_sun = std::make_shared<dae::GameObject>();
	go_sun->AddComponent<dae::RenderComponent>()->SetTexture("sun.png");

	// create earth
	auto go_earth = std::make_shared<dae::GameObject>();
	go_earth->AddComponent<dae::RenderComponent>()->SetTexture("earth.png");
	
	// add earth as child of sun
	go_sun->AddChild(go_earth);

	// create orbiters
	auto orbit_center{ std::make_shared<dae::Orbiter>(70.0f) };
	orbit_center->SetOrbitPosition(glm::vec2(250,250));

	auto orbit_sun{ std::make_shared<dae::Orbiter>(150.0f, go_sun->GetTransform()) };
	orbit_sun->SetOrbitSpeed(50.0f);

	// add orbiter to sun
	go_sun->AddComponent(orbit_center);

	// add orbiter to earth
	go_earth->AddComponent(orbit_sun);

	// add celestial bodies to scene
	scene.Add(go_sun);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}