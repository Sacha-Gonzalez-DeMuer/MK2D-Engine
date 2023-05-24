#pragma once
#include <string>
#include <vector>


namespace dae
{
	class GridGraph;
	class PacLevelLoader final
	{
		using PacLevelData = std::vector<std::string>;



	public:
		PacLevelLoader() = default;
		~PacLevelLoader() = default;
		GridGraph* LoadLevel(const PacLevelData& data);
	};
}

