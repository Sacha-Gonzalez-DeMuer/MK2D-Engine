#include "SceneManager.h"
#include "Scene.h"
#include <algorithm>
#include <iostream>

void dae::SceneManager::Start()
{
	for (auto& scene : m_scenes)
	{
		scene->Start();
	}
}

void dae::SceneManager::Update()
{
	m_ActiveScene->Update();
}

void dae::SceneManager::Render() const
{
	m_ActiveScene->Render();
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	auto scene = std::find_if(m_scenes.begin(), m_scenes.end(), [&name](const std::shared_ptr<Scene>& scene)
		{
		return scene->GetName() == name;
	});

	if (scene != m_scenes.end())
		m_ActiveScene = *scene;
	else
		std::cout << "Scene with name: " << name << " not found!\n";
}

void dae::SceneManager::DeleteObjectsMarkedForDestruction()
{
	for (auto& scene : m_scenes)
	{
		scene->DeleteObjectsMarkedForDestruction();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}


