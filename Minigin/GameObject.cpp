#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"

void dae::GameObject::AddChild(std::shared_ptr<GameObject> child)
{
	m_children.emplace_back(child);
}

void dae::GameObject::Start()
{
	for (auto& child_gameobject : m_children)
		child_gameobject->Start();

	for (auto& component : m_components)
		component->Start();
}

void dae::GameObject::Awake()
{
	for (auto& child_gameobject : m_children)
		child_gameobject->Awake();

	for (auto& component : m_components)
		component->Awake();
}

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	for (auto& child_gameobject : m_children)
		child_gameobject->Update();

	for (auto& component : m_components)
		component->Update();
}

void dae::GameObject::Render() const
{
	//const auto& pos = m_transform.GetPosition();
	//Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);

	for (auto& child_gameobject : m_children)
		child_gameobject->Render();

	for (auto& component : m_components)
		component->Render();
}


void dae::GameObject::SetPosition(float x, float y)
{
	m_transform->SetLocalPosition(x, y);
}

glm::vec2 dae::GameObject::GetPosition() const
{
	return m_transform->GetWorldPosition();
}

void dae::GameObject::AddComponent(std::shared_ptr<Component> component)
{
	component->SetOwner(weak_from_this());
	m_components.emplace_back(component);
}
