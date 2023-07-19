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
		RenderComponent() :m_texture{ nullptr }, m_size{0} {};

		void SetTexture(std::shared_ptr<Texture2D> texture) { m_texture = texture; };
		void SetTexture(const std::string filename) {
			m_texture = ResourceManager::Get().LoadTexture(filename);
			m_filename = filename;
		};

		void SetSize(const glm::vec2& size) { m_size = size; };

		const std::string& GetFilename() const { return m_filename; };
		std::shared_ptr<Texture2D> GetTexture() const { return m_texture; };
		virtual void Render() const override;

	protected:
		std::shared_ptr<Texture2D> m_texture;
		std::string m_filename;
		glm::vec2 m_size;
	};
}


