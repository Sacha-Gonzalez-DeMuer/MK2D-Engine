#include "RenderComponent.h"
#include "Renderer.h"

namespace dae
{
	void RenderComponent::Render() const
	{
		Renderer::GetInstance().RenderTexture(*m_texture, m_gameObject.lock()->GetWorldPosition().x, m_gameObject.lock()->GetWorldPosition().y);
	}
}

 