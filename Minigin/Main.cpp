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
#include "TextComponent.h"
#include "Scene.h"
#include "InputManager.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "FPSCounter.h"
#include <chrono>


void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	auto renderComponent{ std::make_shared<dae::RenderComponent>() };
	go->AddComponent<dae::RenderComponent>()->SetTexture("background.tga");
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	renderComponent = std::make_shared<dae::RenderComponent>();
	go->AddComponent<dae::RenderComponent>()->SetTexture("logo.tga");
	go->SetPosition(216, 180);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	
	auto go_assignment = std::make_shared<dae::GameObject>();
	go_assignment->SetPosition(80, 20);
	go_assignment->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	scene.Add(go_assignment);


	auto go_fps_counter = std::make_shared<dae::GameObject>();
	go_fps_counter->AddComponent<dae::TextComponent>("FPS", font);
	go_fps_counter->AddComponent<dae::FPSCounter>();
	go_fps_counter->SetPosition(0, 0);
	scene.Add(go_fps_counter);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}