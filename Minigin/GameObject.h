#pragma once
#include <memory>
#include "Transform.h"
#include "Component.h"
#include "IObject.h"
#include <vector>
#include <string>

namespace dae
{
	class Texture2D;
	class Component;

	class GameObject final : public IObject, public std::enable_shared_from_this<GameObject>
	{
	public:
		GameObject() : m_transform{ std::make_shared<Transform>(weak_from_this()) }, m_parent{} {};

		~GameObject() {};
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		virtual void Start() override;
		virtual void Update() override;
		virtual void Render() const override;

		void SetLocalPosition(float x, float y);
		void SetLocalPosition(const glm::vec2& position);

		glm::vec2 GetWorldPosition() const;


		template <typename TComponent, typename... Args> std::shared_ptr<TComponent> AddComponent(Args&&... args);;
		template <typename TComponent>	std::shared_ptr<TComponent> AddComponent();
		template <typename TComponent>  std::shared_ptr<TComponent> GetComponent() const;
		template <typename TComponent>	void RemoveComponent();

		std::weak_ptr<GameObject> GetParent() { return m_parent; };

		void SetParent(std::weak_ptr<GameObject> parent, bool keepWorldPosition);
		void AddChild(std::shared_ptr<GameObject> child);
		void RemoveChild(std::shared_ptr<GameObject> child);
		std::vector<std::shared_ptr<GameObject>>& GetChildren() { return m_children; };
		std::shared_ptr<Transform> GetTransform() { return m_transform; };

	private:
		std::shared_ptr<Transform> m_transform;

		std::weak_ptr<GameObject> m_parent;

		std::vector<std::shared_ptr<GameObject>> m_children{};
		std::vector<std::shared_ptr<Component>> m_components{};
	};

	template <typename TComponent, typename... Args>
	std::shared_ptr<TComponent> GameObject::AddComponent(Args&&... args)
	{
		static_assert(std::is_base_of<Component, TComponent>::value, "Template type must be a component.");

		auto newComponent = std::make_shared<TComponent>(std::forward<Args>(args)...);
		newComponent->SetOwner(this);

		m_components.emplace_back(newComponent);
		return newComponent;
	}

	template<typename TComponent>
	inline std::shared_ptr<TComponent> GameObject::AddComponent()
	{
		static_assert(std::is_base_of<Component, TComponent>::value, "Template type must be a component.");

		auto new_component{ std::make_shared<TComponent>() };
		std::dynamic_pointer_cast<Component>(new_component)->SetOwner(this);
		m_components.emplace_back(new_component);

		return new_component;
	}

	template<typename TComponent>
	inline std::shared_ptr<TComponent> GameObject::GetComponent() const
	{
		static_assert(std::is_base_of<Component, TComponent>::value, "Template type must be a component.");

		// Find the first component in the vector that is of type T*
		auto it = std::find_if(m_components.begin(), m_components.end(), [](std::shared_ptr<Component> component) {
			return std::dynamic_pointer_cast<TComponent>(component) != nullptr;
			});

		// If a component of type T* is found, return it
		if (it != m_components.end())
			return std::dynamic_pointer_cast<TComponent>(*it);

		return nullptr;
	}

	template<typename TComponent>
	inline void GameObject::RemoveComponent()
	{
		static_assert(std::is_base_of<Component, TComponent>::value, "Template type must be a component.");

		// Find the first component in the vector that is of type T*
		auto it = std::find_if(m_components.begin(), m_components.end(), [](Component* component) {
			return dynamic_cast<TComponent*>(component) != nullptr;
			});

		m_components.erase(it);
	}
}
