#include "Debug.h"
#include <algorithm>
#include "GridGraph.h"
#include "Renderer.h"
#include "GraphNode.h"

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

	void Debug::DrawGrid(GridGraph* grid) const
	{
		const auto& nodes = grid->GetAllNodes();

		for (const auto& node : nodes)
		{
			Renderer::GetInstance()
				.DrawRect
				(node->GetPosition()
				, static_cast<float>(grid->GetCellSize()), static_cast<float>(grid->GetCellSize())
				, {255,255,255,255});
		}
	}
}
