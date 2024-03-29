#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

void dae::ResourceManager::Init(const std::string& dataPath)
{
	m_dataPath = dataPath;

	if (TTF_Init() != 0)
		throw std::runtime_error(std::string("Failed to initialize fonts: ") + SDL_GetError());
}

std::shared_ptr<dae::Texture2D> dae::ResourceManager::LoadTexture(const std::string& file) const
{
	const auto& fullPath = m_dataPath + file;

	if(m_textureCache.contains(fullPath))
		return m_textureCache.at(fullPath);

	auto texture = IMG_LoadTexture(Renderer::Get().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr)
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());

	auto pTexture = std::make_shared<Texture2D>(texture);
	m_textureCache[fullPath] = pTexture;

	return pTexture;
}

std::shared_ptr<dae::Font> dae::ResourceManager::LoadFont(const std::string& file, unsigned int size) const
{
	const auto& fullPath = m_dataPath + file;

	if (m_fontCache.contains(fullPath))
		return m_fontCache.at(fullPath);

	auto pFont = std::make_shared<Font>(fullPath, size);
	m_fontCache[fullPath] = pFont;

	return pFont;
}
