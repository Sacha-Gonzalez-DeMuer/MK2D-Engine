#pragma once
#include "Component.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include <string>

namespace dae
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent() = default;

		void SetTexture(std::shared_ptr<Texture2D> texture) { m_texture = texture; };
		void SetTexture(const std::string filename) {
			m_texture = ResourceManager::GetInstance().LoadTexture(filename);
		};

		virtual void Render() const override;

	private:
		std::shared_ptr<Texture2D> m_texture{nullptr};
	};
}


