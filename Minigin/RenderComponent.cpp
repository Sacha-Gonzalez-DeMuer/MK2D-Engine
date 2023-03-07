#include "RenderComponent.h"
#include "Renderer.h"

namespace dae
{
	void RenderComponent::Render() const
	{
		Renderer::GetInstance().RenderTexture(*m_texture, m_owner.lock()->GetPosition().x, m_owner.lock()->GetPosition().y);
	}
}

 