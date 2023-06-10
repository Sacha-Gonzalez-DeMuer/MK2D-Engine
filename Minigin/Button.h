#pragma once
#include "RenderComponent.h"
#include "Delegate.h"

namespace dae
{
	class Button : public RenderComponent
	{
	public:
		Button();
		Button(const Button& other) = delete;
		Button(Button&& other) noexcept = delete;
		Button& operator=(const Button& other) = delete;
		Button& operator=(Button&& other) noexcept = delete;
		virtual ~Button() = default;

		void Update() override;
		void Start() override;

		Delegate<> OnClick;

	private:

		int m_btnMinX;
		int m_btnMaxX;
		int m_btnMinY;
		int m_btnMaxY;
	};
}

