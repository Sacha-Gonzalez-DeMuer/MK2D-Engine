#include "Scene.h"
#include "GameObject.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

void dae::Scene::DeleteObjectsMarkedForDestruction()
{
	for (auto it = m_objects.begin(); it != m_objects.end();)
	{
		if ((*it)->IsMarkedForDestruction())
			Remove(*it);
		else
			++it;
	}
}

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	if (auto collider = object->GetComponent<ICollider>())
		m_collisionObjects.emplace_back(collider);

	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
	m_collisionObjects.erase(std::remove(m_collisionObjects.begin(), m_collisionObjects.end(), object->GetComponent<ICollider>()), m_collisionObjects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
	m_collisionObjects.clear();
}

void dae::Scene::Start()
{
	for (auto& object : m_objects)
	{
		object->Start();
	}
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}


	// for each collision object
	for (size_t i = 0; i < m_collisionObjects.size(); i++)
	{
		// check collision with all other collision objects
		for (size_t j = i + 1; j < m_collisionObjects.size(); j++)
		{
			if (m_collisionObjects[i]->Intersects(m_collisionObjects[j]))
			{
				m_collisionObjects[i]->GetOwner()->OnCollision(*m_collisionObjects[j]);
				m_collisionObjects[j]->GetOwner()->OnCollision(*m_collisionObjects[i]);
			}
		}
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}



