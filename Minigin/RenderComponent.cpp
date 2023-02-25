#include "RenderComponent.h"
#include "Renderer.h"

namespace dae
{
	void RenderComponent::Render() const
	{
		Renderer::GetInstance().RenderTexture(*m_texture, GetPosition().x, GetPosition().y);
	}
}

 