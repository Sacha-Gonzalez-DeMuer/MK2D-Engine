#pragma once
#include "Component.h"
#include "GameObject.h"
namespace dae
{
	class Transform;
	// Orbiter will make its owning gameobject rotate in circles around a defined transform at a defined distance
	// if no transform is set, it will orbit it's own original position
	class Orbiter : public Component
	{
	public:
		Orbiter() = delete;
		Orbiter(float orbitDistance)
			: m_orbitDistance{ orbitDistance }, m_orbitCenter{ nullptr }, m_orbitPosition{ glm::vec2{0,0} } {};
		Orbiter(float orbitDistance, std::shared_ptr<Transform> orbitCenter)
			: m_orbitDistance{ orbitDistance }, m_orbitCenter{ orbitCenter }, m_orbitPosition{ orbitCenter->GetWorldPosition() } {};

		void SetOrbitPosition(const glm::vec2& orbitPos) { m_orbitPosition = orbitPos; };
		void SetOrbitSpeed(float speed) { m_orbitSpeed = speed; };
		void Update() override;

	private:
		float m_orbitDistance;
		float m_angle{ 0 };
		float m_orbitSpeed{ 100.0f };
		std::shared_ptr<Transform> m_orbitCenter;

		glm::vec2 m_orbitPosition;
	};

}

