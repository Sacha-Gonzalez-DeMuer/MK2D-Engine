#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"

void dae::GameObject::SetParent(std::shared_ptr<GameObject> parent, bool keepWorldPosition)
{	
	if (m_parent == nullptr)
		SetLocalPosition(m_transform->GetWorldPosition());
	else
		if (keepWorldPosition)
			SetLocalPosition((GetWorldPosition() - m_parent->GetWorldPosition()));

	if (m_parent)
		m_parent->RemoveChild(std::shared_ptr<GameObject>(this));
	m_parent = parent;
	if (m_parent)
		m_parent->AddChild(std::shared_ptr<GameObject>(this));

}

void dae::GameObject::AddChild(std::shared_ptr<GameObject> child)
{
	m_children.emplace_back(child);
}

void dae::GameObject::RemoveChild(std::shared_ptr<GameObject> child)
{
	auto it = std::find(m_children.begin(), m_children.end(), child);
	if (it != m_children.end())
	{
		child->SetParent(nullptr, false);
		m_children.erase(it);
	}
}

void dae::GameObject::Start()
{
	if(!m_transform) m_transform = std::make_shared<Transform>(weak_from_this());

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


void dae::GameObject::SetLocalPosition(const glm::vec2& position)
{
	m_transform->SetLocalPosition(position.x, position.y);
}

void dae::GameObject::SetLocalPosition(float x, float y)
{
	// temporary fix for transform not being linked to owner at construction
	// weak_to_this() only works when gameobject is fully constructed
	// can't find a way to construct transform in gameobjects constructor because of that
	if(!m_transform) m_transform = std::make_shared<Transform>(weak_from_this());



	m_transform->SetLocalPosition(x, y);
}



glm::vec2 dae::GameObject::GetWorldPosition() const
{
	return m_transform->GetWorldPosition();
}

void dae::GameObject::AddComponent(std::shared_ptr<Component> component)
{
	component->SetOwner(weak_from_this());
	m_components.emplace_back(component);
}
