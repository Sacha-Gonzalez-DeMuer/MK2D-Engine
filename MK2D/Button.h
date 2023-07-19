#pragma once
#include "RenderComponent.h"
#include "Delegate.h"

namespace dae
{
	class Button : public RenderComponent
	{
	public:
		Button();
		virtual ~Button() = default;

		void Update() override;
		void Start() override;

		Delegate<> OnClick;

	private:
		int m_btnMinX;
		int m_btnMaxX;
		int m_btnMinY;
		int m_btnMaxY;
		float m_reclickDelay;
	};
}

