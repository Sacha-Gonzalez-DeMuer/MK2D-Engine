#pragma once
#include <glm/glm.hpp>
#include <memory>

namespace dae
{
	class Transform final
	{
	public:
		Transform();
		Transform(const glm::vec3& pos);

		const glm::vec3& GetLocalPosition() const { return m_position; };
		const glm::vec3& GetWorldPosition() const;

		void SetPosition(float x, float y, float z);

	private:
		bool m_needsUpdate;
		std::shared_ptr<Transform> m_parent;
		glm::vec3 m_position;
	};
}
