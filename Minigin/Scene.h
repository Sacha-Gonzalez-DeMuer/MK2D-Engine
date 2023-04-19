#pragma once
#include "SceneManager.h"
#include "ICollider.h"

namespace dae
{
	class GameObject;
	class Scene final : public IObject
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		virtual void Start() override;
		virtual void Update() override;
		virtual void Render() const override;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void DeleteObjectsMarkedForDestruction();

	private: 
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector <std::shared_ptr<GameObject>> m_objects{};
		std::vector <std::shared_ptr<ICollider>> m_collisionObjects{};

		static unsigned int m_idCounter; 
	};

}
