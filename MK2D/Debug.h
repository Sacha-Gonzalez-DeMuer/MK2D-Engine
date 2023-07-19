#pragma once
#include "Singleton.h"
#include "imgui.h"
#include "imgui_plot.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <memory>
namespace dae
{
	class GridGraph;
	class Font;
	class Debug : public dae::Singleton<Debug>
	{
	public:
		Debug();

		void PlotData(const char* title, const std::vector<float>& x, const std::vector<float>& y);
		void DrawGrid( GridGraph* grid) const;
		void DrawDebugText(const std::string& text, const glm::vec2& pos) const;
	private:
		std::shared_ptr<Font> m_font;
	};
}

