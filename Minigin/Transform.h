#pragma once
#include <glm/glm.hpp>
#include <memory>

namespace dae
{
	class GameObject;
	class Transform final
	{
	public:
		Transform() = delete;
		Transform(std::weak_ptr<GameObject> owner);

		const glm::vec2& GetLocalPosition() const { return m_localPosition; };
		const glm::vec2& GetWorldPosition();

		void UpdateWorldPosition();

		void SetLocalPosition(float x, float y);
		void SetLocalPosition(const glm::vec2& toPos);

	private:
		bool m_Dirty;

		glm::vec2 m_localPosition;
		glm::vec2 m_worldPosition;

		std::weak_ptr<GameObject> m_owner;
	};
}
