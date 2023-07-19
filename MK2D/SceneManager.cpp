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

void dae::SceneManager::RemoveScene(const std::string& name)
{
	m_scenesToRemove.emplace_back(name);
}

void dae::SceneManager::DeleteObjectsMarkedForDestruction()
{
	for (auto& scene : m_scenes)
	{
		scene->DeleteObjectsMarkedForDestruction();
	}

	for (auto sceneName : m_scenesToRemove)
	{
		auto sceneToRemove = std::find_if(m_scenes.begin(), m_scenes.end(), [&sceneName](const std::shared_ptr<Scene>& scene)
			{
				return scene->GetName() == sceneName;
			});
		if (sceneToRemove != m_scenes.end())
			m_scenes.erase(sceneToRemove);
	}
}

bool dae::SceneManager::HasScene(const std::string& name) const
{
	auto scene = std::find_if(m_scenes.begin(), m_scenes.end(), [&name](const std::shared_ptr<Scene>& scene)
		{
			return scene->GetName() == name;
		});

	return scene != m_scenes.end();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	if (HasScene(name))
		throw std::runtime_error("Scene with name: " + name + " already exists!");

	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}


