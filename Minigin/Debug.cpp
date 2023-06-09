#include "Debug.h"
#include <algorithm>
#include "GridGraph.h"
#include "Renderer.h"
#include "GraphNode.h"
#include "imgui.h"
#include "imgui_plot.h"
#include <SDL_ttf.h>
#include "Font.h"
#include <stdexcept>
#include "Texture2D.h"
#include "ResourceManager.h"

namespace dae
{
	Debug::Debug()
		: m_font(dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 12))
	{}

	void Debug::PlotData(const char* title, const std::vector<float>& x, const std::vector<float>& y)
	{
		ImGui::PlotConfig config;
		config.values.xs = x.data();
		config.values.ys = y.data();
		config.values.count = static_cast<int>(y.size());
		config.scale.min = *std::min_element(y.begin(), y.end());
		config.scale.max = *std::max_element(y.begin(), y.end());
		config.tooltip.show = true;
		config.frame_size = ImVec2(200, 100);

		ImGui::Plot(title, config);
	}

	void Debug::DrawGrid(GridGraph* grid) const
	{
		const auto& nodes = grid->GetAllNodes();

		for (const auto& node : nodes)
		{
			Renderer::GetInstance()
				.DrawRect
				(node->GetLocalPosition()
				, static_cast<float>(grid->GetCellSize()), static_cast<float>(grid->GetCellSize())
				, {255,255,255,255});

		}
	}
	void Debug::DrawDebugText(const std::string& text, const glm::vec2& pos) const
	{
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), text.c_str(), {255,255,255,255});
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		SDL_FreeSurface(surf);
		auto textTex = std::make_shared<Texture2D>(texture);
		Renderer::GetInstance().RenderTexture(*textTex, pos.x, pos.y);
	}
}
