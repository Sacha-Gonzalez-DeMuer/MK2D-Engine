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
#include "CacheTrasher.h"
#include "CacheTrashDebugger.h"


#include <chrono>
#include <iostream>

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


	// assignment text
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	
	auto go_assignment = std::make_shared<dae::GameObject>();
	go_assignment->SetLocalPosition(80, 20);
	go_assignment->AddComponent<dae::TextObject>("Programming 4 Assignment", font);
	scene.Add(go_assignment);

	// make fps counter
	auto go_fps_counter = std::make_shared<dae::GameObject>();
	go_fps_counter->AddComponent<dae::FPSCounter>();
	go_fps_counter->AddComponent<dae::TextObject>("FPS", font);
	go_fps_counter->SetLocalPosition(0, 0);
	scene.Add(go_fps_counter);

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


	//////// TRASH THE CACHE
	//constexpr int nrMeasurements{ 10 };
	//constexpr int maxStep{ 1024 };
	//constexpr int stepSize{ 2 };
	//constexpr int nrObjects{ 10'000'000 };
	//constexpr int multiplication{2};
	//auto cache_trasher{ std::make_unique<dae::CacheTrasher>() };
	//
	//// Exercise 1
	//std::vector<int> int_cache(nrObjects);

	//auto avgTimes = cache_trasher->MeasureMultiplication<int>(int_cache, multiplication, nrMeasurements, stepSize, maxStep);

	//// print results
	//std::cout << "1.0" << '\n';
	//for (int i = 0; i < static_cast<int>(avgTimes.size()); ++i)
	//{
	//	std::cout << (1 << i) << ": " << avgTimes[i] << " microseconds" << '\n';
	//}
	//
	//// Exercise 2
	//std::vector<W04_DataTypes::GameObject> transform_objects{ nrObjects };
	//std::vector<W04_DataTypes::GameObjectAlt> transform_pointer_objects{ nrObjects };

	//// 2.1
	//avgTimes = cache_trasher->MeasureMultiplication<W04_DataTypes::GameObject>(transform_objects, multiplication, nrMeasurements, stepSize, maxStep);

	//// print results
	//std::cout << "2.1" << '\n';
	//for (int i = 0; i < static_cast<int>(avgTimes.size()); ++i)
	//{
	//	std::cout << (1 << i) << ": " << avgTimes[i] << " microseconds" << '\n';
	//}

	//// 2.2
	//avgTimes = cache_trasher->MeasureMultiplication<W04_DataTypes::GameObjectAlt>(transform_pointer_objects, multiplication, nrMeasurements, stepSize, maxStep);

	//// print results
	//std::cout << "2.2" << '\n';
	//for (int i = 0; i < static_cast<int>(avgTimes.size()); ++i)
	//{
	//	std::cout << (1 << i) << ": " << avgTimes[i] << " microseconds" << '\n';
	//}

	//// calculate the X values
	//std::vector<float> steps(avgTimes.size());
	//for (int i = 0; i < avgTimes.size(); ++i)
	//{
	//	steps[i] = static_cast<float>(std::pow(2, i));
	//}

	// Debug::GetInstance().PlotData(steps, avgTimes);


	auto cache_trasher_go{ std::make_shared<dae::GameObject>() };
	cache_trasher_go->AddComponent<dae::CacheTrashDebugger>();
	scene.Add(cache_trasher_go);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}

