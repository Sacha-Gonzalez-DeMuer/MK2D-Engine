#pragma once
#include <memory>
#include "Transform.h"
#include "Component.h"
#include "IObject.h"
#include <vector>
#include <string>
#include <stdexcept>

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

		template <typename TComponent>  std::shared_ptr<TComponent> AddComponent();
		template <typename TComponent, typename... Args> std::shared_ptr<TComponent> AddComponent(Args&&... constructorArguments);
		template <typename TComponent>  std::shared_ptr<TComponent> GetComponent() const;
		template <typename TComponent> void RemoveComponent();

		void AddChild(std::shared_ptr<GameObject> child);

		const Transform& GetTransform() const { return m_transform; };
		

	private:
		Transform m_transform{};

		std::vector<std::shared_ptr<GameObject>> m_children{};
		std::vector<std::shared_ptr<Component>> m_components{};
	};



	// Template Declarations
	template<typename TComponent>
	inline std::shared_ptr<TComponent> GameObject::AddComponent()
	{
		static_assert(std::is_base_of<Component, TComponent>::value, "Template type must be a component.");

		auto new_component{ std::make_shared<TComponent>(weak_from_this()) };
		m_components.emplace_back(new_component);

		return new_component;
	}

	template<typename TComponent, typename ...Args>
	inline std::shared_ptr<TComponent> GameObject::AddComponent(Args && ...constructorArguments)
	{
		static_assert(std::is_base_of<Component, TComponent>::value, "Template type must be a component.");

		return std::make_shared<TComponent>(std::forward<Args>(constructorArguments)...);
	}

	template<typename TComponent>
	inline std::shared_ptr<TComponent> GameObject::GetComponent() const
	{
		static_assert(std::is_base_of<Component, TComponent>::value, "Template type must be a component.");

		// Find the first component in the vector that is of type T*
		auto it = std::find_if(m_components.begin(), m_components.end(), [](std::shared_ptr<Component> component) {
			return std::dynamic_pointer_cast<TComponent>(component) != nullptr;
			});

		//// 
		//if (it == m_components.end())
		//	throw std::runtime_error("Gameobject does not contain component of type" + typeid(TComponent).name());

		return std::dynamic_pointer_cast<TComponent>(*it);
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
