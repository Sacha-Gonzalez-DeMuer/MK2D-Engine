#pragma once
#include <string>
#include "PacData.h"

namespace dae
{
	class PacJSONParser final
	{
	public:
		PacJSONParser(const std::string& dataFile);

		bool LoadLevel(int levelIdx, PacData::PacLevelData& levelContainer);
		bool LoadGameData(PacData::PacGameData& gameDataContainer);

	private:
		std::string m_dataFile;
	};
}
