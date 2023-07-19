#include "RenderComponent.h"
#include "Renderer.h"

namespace dae
{
	void RenderComponent::Render() const
	{
		if(m_size.x == 0 && m_size.y == 0)
			Renderer::Get().RenderTexture(*m_texture, m_gameObject.lock()->GetWorldPosition().x, m_gameObject.lock()->GetWorldPosition().y);
		else
			Renderer::Get().RenderTexture(*m_texture, m_gameObject.lock()->GetWorldPosition().x, m_gameObject.lock()->GetWorldPosition().y, m_size.x, m_size.y);
	}
}

 