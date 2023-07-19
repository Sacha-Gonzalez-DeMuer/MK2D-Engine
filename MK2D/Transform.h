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
		~Transform() { m_owner = nullptr; };
		Transform(GameObject* owner);

		const glm::vec2& GetLocalPosition() const { return m_localPosition; };
		const glm::vec2& GetWorldPosition();

		void UpdateWorldPosition();

		void SetLocalPosition(float x, float y);
		void SetLocalPosition(const glm::vec2& toPos);

		void SetPositionDirty();

	private:
		bool m_isPositionDirty;

		glm::vec2 m_localPosition;
		glm::vec2 m_worldPosition;

		GameObject* m_owner;
	};
}
