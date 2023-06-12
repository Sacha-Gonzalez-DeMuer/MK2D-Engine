#include "Toggle.h"
#include "ResourceManager.h"


dae::Toggle::Toggle(const std::string& toggleOnTexture, const std::string& toggleOffTexture)
	: m_isOn(false), m_pOnTexture(ResourceManager::Get().LoadTexture(toggleOnTexture)), m_pOffTexture(ResourceManager::Get().LoadTexture(toggleOffTexture))
{
	SetTexture(m_pOffTexture);
	OnClick.AddFunction([this]() {
		m_isOn = !m_isOn;
		SetTexture(m_isOn ? m_pOnTexture : m_pOffTexture);
		});
}
