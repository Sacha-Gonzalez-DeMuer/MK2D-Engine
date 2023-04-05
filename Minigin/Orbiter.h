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
			: m_orbitDistance{ orbitDistance } {};

		void SetOrbitSpeed(float speed) { m_orbitSpeed = speed; };
		void Update() override;

	private:
		float m_orbitDistance;
		float m_angle{ 0 };
		float m_orbitSpeed{ 100.0f };
	};

}

