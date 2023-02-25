#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Awake()
{
	for (auto& scene : m_scenes)
	{
		scene->Awake();
	}
}

void dae::SceneManager::Start()
{
	for (auto& scene : m_scenes)
	{
		scene->Start();
	}
}

void dae::SceneManager::Update()
{
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void dae::SceneManager::Render() const
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

void dae::SceneManager::OnDestroy()
{
	for (auto& scene : m_scenes)
	{
		scene->OnDestroy();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}


