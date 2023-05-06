#pragma once
#include "Singleton.h"
#include "imgui.h"
#include "imgui_plot.h"
#include <string>
#include <vector>


namespace dae
{
	class GridGraph;
	class Debug : public dae::Singleton<Debug>
	{
	public:
		Debug() {};

		void PlotData(const char* title, const std::vector<float>& x, const std::vector<float>& y);
		void DrawGrid( GridGraph* grid) const;
	private:

	};

}

