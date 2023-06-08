#pragma once
#include <string>
#include "PacData.h"

class PacJSONParser final
{
public:
	PacJSONParser(const std::string& dataFile);

	bool LoadLevel(int levelIdx, PacData::PacLevelData& levelContainer);

private:
	void ParseJSONData(const std::string& dataFile);
	std::string dataFile;
};

