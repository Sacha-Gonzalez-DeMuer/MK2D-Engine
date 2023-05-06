#pragma once
#include "Component.h"
#include "CGraphNodeNavigator.h"

namespace dae
{
	class RenderComponent;
	class PlayerController : public Component
	{
	public:
		PlayerController(std::shared_ptr<GridNavComponent> pNavigator);
		~PlayerController() = default;
		void Update() override;
		void Render() const override;

	private:
		void InitializeInput();

		std::shared_ptr<GridNavComponent> m_pNavigator;
		float m_Speed;
	};
}


