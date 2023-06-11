#include "GameObject.h"
#pragma once
#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"
#include "SceneManager.h"

void dae::GameObject::SetParent(std::weak_ptr<GameObject> parent, bool keepWorldPosition)
{
	if (parent.lock() == m_parent.lock()) return;

	if (parent.expired()) {
		SetLocalPosition(GetWorldPosition());
	}
	else {
		if (keepWorldPosition) {
			SetLocalPosition(m_transform->GetLocalPosition() - parent.lock()->GetWorldPosition());
		}
		m_transform->SetPositionDirty();
	}
	if (auto prevParent = m_parent.lock()) {
		prevParent->RemoveChild(shared_from_this());
	}
	m_parent = parent;
	if (auto newParent = m_parent.lock()) {
		newParent->AddChild(shared_from_this());
	}
}
void dae::GameObject::AddChild(std::shared_ptr<GameObject> child)
{
	if (child == nullptr) return;
	if (child->m_parent.lock().get() == this) return;

	if (auto prevParent = child->m_parent.lock()) {
		prevParent->RemoveChild(child);
	}
	m_children.push_back(child);
	child->m_parent = shared_from_this();
}

void dae::GameObject::RemoveChild(std::shared_ptr<GameObject> child)
{
	if (child == nullptr) return;

	auto it = std::find(m_children.begin(), m_children.end(), child);
	if (it != m_children.end()) {
		m_children.erase(it);
		if (auto parent = child->m_parent.lock()) {

			child->m_parent.lock()->GetTransform()->SetPositionDirty();
			child->m_parent.reset();
		}
	}
}

void dae::GameObject::Start()
{
	for (auto& child_gameobject : m_children)
		child_gameobject->Start();

	for (auto& component : m_components)
		component->Start();
}

void dae::GameObject::Update()
{
	if (!m_isActive) return;

	for (auto& child_gameobject : m_children)
		child_gameobject->Update();

	for (auto& component : m_components)
		component->Update();
}

void dae::GameObject::Render() const
{
	if (!m_isActive) return;

	for (auto& component : m_components)
		component->Render();

	for (auto& child_gameobject : m_children)
		child_gameobject->Render();
}

void dae::GameObject::OnCollisionEnter(ICollider& other)
{
	if (!m_isActive) return;

	for (auto component : m_components)
		component->OnCollisionEnter(other);
}

void dae::GameObject::SetLocalPosition(float x, float y)
{
	m_transform->SetLocalPosition(x, y);
}

void dae::GameObject::SetLocalPosition(const glm::vec2& position)
{
	m_transform->SetLocalPosition(position.x, position.y);
}

glm::vec2 dae::GameObject::GetWorldPosition() const
{
	return m_transform->GetWorldPosition();
}
