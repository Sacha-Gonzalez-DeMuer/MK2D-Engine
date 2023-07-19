#pragma once
#include <string>
#include <memory>
#include "Singleton.h"

#include <unordered_map>
namespace dae
{
	class Texture2D;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	private:
		using TextureCache = std::unordered_map<std::string, std::shared_ptr<Texture2D>>;
		using FontCache = std::unordered_map<std::string, std::shared_ptr<Font>>;
	public:
		void Init(const std::string& data);
		std::shared_ptr<Texture2D> LoadTexture(const std::string& file) const;
		std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size) const;
		const std::string& GetDataPath() const { return m_dataPath; }
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_dataPath;

		mutable TextureCache m_textureCache;
		mutable FontCache m_fontCache;
	};
}
