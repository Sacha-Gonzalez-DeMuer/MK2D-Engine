#pragma once
#include "Component.h"

namespace dae
{
	class TextComponent;

	class FPSCounter final : public Component
	{
	public:
		FPSCounter() = default;

		void Start() override;
		void Update() override;
		void Render() const override;

	private:
		std::shared_ptr<TextComponent> m_textObject;
		float m_framesPerSecond{ 0.f };
		float m_timer{ 0.f };
		int m_frameCount{ 0 };
	};
}

