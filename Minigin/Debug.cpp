#include "Debug.h"
#include <algorithm>


namespace dae
{
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
}
