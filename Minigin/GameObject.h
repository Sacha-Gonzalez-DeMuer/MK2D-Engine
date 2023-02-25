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
		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		virtual void Start() override;
		virtual void Awake() override;
		virtual void Update() override;
		virtual void Render() const override;
		virtual void OnDestroy() override;

		void SetPosition(float x, float y);

		template <typename T>  std::shared_ptr<T> AddComponent();
		void AddComponent(std::shared_ptr<Component> component);
		template <typename T>  std::shared_ptr<T> GetComponent() const;
		template <typename T> void RemoveComponent();

		void AddChild(std::shared_ptr<GameObject> child);

		const Transform& GetTransform() const { return m_transform; };


	private:
		Transform m_transform{};

		std::vector<std::shared_ptr<GameObject>> m_children{};
		std::vector<std::shared_ptr<Component>> m_components{};
	};

	template<typename T>
	inline std::shared_ptr<T> GameObject::AddComponent()
	{
		if (std::is_base_of<Component, T>())
		{
			auto new_component{ std::make_shared<T>() };
			std::dynamic_pointer_cast<Component>(new_component)->AttachToGameObject(weak_from_this());
			m_components.emplace_back(new_component);

			return new_component;
		}
		return nullptr;
	}

	template<typename T>
	inline std::shared_ptr<T> GameObject::GetComponent() const
	{
		// Find the first component in the vector that is of type T*
		auto it = std::find_if(m_components.begin(), m_components.end(), [](std::shared_ptr<Component> component) {
			return std::dynamic_pointer_cast<T>(component) != nullptr;
			});

		// If a component of type T* is found, return it
		if (it != m_components.end())
			return std::dynamic_pointer_cast<T>(*it);

		return nullptr;
	}

	template<typename T>
	inline void GameObject::RemoveComponent()
	{
		// Find the first component in the vector that is of type T*
		auto it = std::find_if(m_components.begin(), m_components.end(), [](Component* component) {
			return dynamic_cast<T*>(component) != nullptr;
			});

		m_components.erase(it);
	}
}
