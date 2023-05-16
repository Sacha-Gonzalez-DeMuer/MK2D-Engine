#pragma once
#include "Component.h"
#include "GridNavComponent.h"
namespace dae
{
	class RenderComponent;
	class PlayerController : public Component
	{
	public:
		PlayerController(std::shared_ptr<GridNavComponent> pNavigator, const float speed);
		~PlayerController() = default;

	protected:
		void InitializeInput();

		std::shared_ptr<GridNavComponent> m_pNavigator;
		float m_Speed;
	};
}


