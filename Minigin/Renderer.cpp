#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl2.h"
#include <SDL_ttf.h>
#include "../3rdParty/SDL2_gfx/SDL_gfxPrimitives.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();

}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);


	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();

	SceneManager::Get().Render();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	
	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::DrawLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& col) const
{
	//set draw color to white
	SDL_SetRenderDrawColor(m_renderer, static_cast<Uint8>(col.x), static_cast<Uint8>(col.y), static_cast<Uint8>(col.z), static_cast<Uint8>(col.w));
	SDL_RenderDrawLine(m_renderer, static_cast<int>(p1.x), static_cast<int>(p1.y), static_cast<int>(p2.x), static_cast<int>(p2.y));
}

void dae::Renderer::DrawRect(const glm::vec2& pos, float width, float height, const glm::vec4& col) const
{
	SDL_SetRenderDrawColor(m_renderer, static_cast<Uint8>(col.x), static_cast<Uint8>(col.y), static_cast<Uint8>(col.z), static_cast<Uint8>(col.w));

	SDL_Rect r;
	r.x = static_cast<int>(pos.x);
	r.y = static_cast<int>(pos.y);
	r.w = static_cast<int>(width);
	r.h = static_cast<int>(height);
	SDL_RenderDrawRect(m_renderer, &r);
}

void dae::Renderer::FillRect(const glm::vec2& pos, float width, float height, const glm::vec4& col) const
{
	SDL_SetRenderDrawColor(m_renderer, static_cast<Uint8>(col.x), static_cast<Uint8>(col.y), static_cast<Uint8>(col.z), static_cast<Uint8>(col.w));

	SDL_Rect r;
	r.x = static_cast<int>(pos.x);
	r.y = static_cast<int>(pos.y);
	r.w = static_cast<int>(width);
	r.h = static_cast<int>(height);
	SDL_RenderFillRect(m_renderer, &r);
}

void dae::Renderer::FillCircle(const glm::vec2& pos, int radius, const glm::vec4& col) const
{
	SDL_SetRenderDrawColor(m_renderer, static_cast<Uint8 >(col.x), static_cast<Uint8>(col.y), static_cast<Uint8>(col.z), static_cast<Uint8>(col.w));

	for (int y = -radius; y <= radius; y++)
	{
		for (int x = -radius; x <= radius; x++)
		{
			if (x * x + y * y <= radius * radius)
			{
				SDL_RenderDrawPoint(m_renderer, static_cast<int>(pos.x) + x, static_cast<int>(pos.y) + y);
			}
		}
	}
}
inline SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }