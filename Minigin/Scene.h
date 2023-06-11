#pragma once
#include "SceneManager.h"
#include "ICollider.h"
#include <unordered_set>
#include <utility>
namespace dae
{
	class GameObject;
	class Scene final : public IObject
	{
		struct ColliderPairHash
		{
			std::size_t operator()(const std::pair<std::shared_ptr<ICollider>, std::shared_ptr<ICollider>>& pair) const
			{
				// Combine the hashes of the two colliders using a bitwise XOR
				std::size_t hash1 = std::hash<std::shared_ptr<ICollider>>{}(pair.first);
				std::size_t hash2 = std::hash<std::shared_ptr<ICollider>>{}(pair.second);
				return hash1 ^ hash2;
			}
		};
		using CollisionPair = std::pair<
			std::shared_ptr<ICollider>, std::shared_ptr<ICollider>>;

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

		const std::string& GetName() const { return m_name; }
	private: 
		explicit Scene(const std::string& name);

		std::string m_name{};
		std::vector <std::shared_ptr<GameObject>> m_objects{};
		std::vector <std::shared_ptr<ICollider>> m_collisionObjects{};
		std::unordered_set<CollisionPair, ColliderPairHash> m_currentCollisions{};
		std::unordered_set<CollisionPair, ColliderPairHash> m_previousCollisions{};


		bool m_Started{};
		static unsigned int m_idCounter;
	};

}
