#pragma once
#include <vector>
#include <string>
#include <memory>
#include "IObject.h"
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>, public IObject
	{
	public:
		Scene& CreateScene(const std::string& name);

		virtual void Start() override;
		virtual void Update() override;
		virtual void Render() const override;

		void SetActiveScene(const std::string& name);
		void DeleteObjectsMarkedForDestruction();

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;

		std::shared_ptr<Scene> m_ActiveScene{ nullptr };
	};
}
