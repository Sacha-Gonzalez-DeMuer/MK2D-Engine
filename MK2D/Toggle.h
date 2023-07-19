#pragma once
#include "Button.h"

namespace dae
{
	class Toggle final : public Button
	{
	public:
		Toggle(const std::string& toggleOnTexture, const std::string& toggleOffTexture);
		virtual ~Toggle() = default;

		bool IsToggled() const { return m_isOn; }


	private:
		bool m_isOn;
		std::shared_ptr<Texture2D> m_pOnTexture;
		std::shared_ptr<Texture2D> m_pOffTexture;
	};

}

