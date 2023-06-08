#include <fstream>
#include <iostream>
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
#include "PacJSONParser.h"

PacJSONParser::PacJSONParser(const std::string& dataFile)
	: dataFile{dataFile}
{}

bool PacJSONParser::LoadLevel(int levelIdx, PacData::PacLevelData& levelContainer)
{
	using namespace rapidjson;

	std::ifstream ifs(dataFile);
	if (!ifs.is_open()) {
		std::cerr << "Failed to open JSON file: " << dataFile << '\n';
		return false;
	}

	IStreamWrapper isw(ifs);
	Document doc;

	doc.ParseStream(isw);
	if (doc.HasParseError()) {
		std::cerr << "Failed to parse JSON file: " << dataFile << '\n';
		return false;
	}


	if (!doc.HasMember("levels") || doc["levels"].IsArray())
	{
		const Value& levels = doc["levels"];
		const Value& level = levels[levelIdx];

		if (level.HasMember("map") && level["map"].IsArray())
		{
			const Value& mapArray = level["map"];
			for (SizeType j = 0; j < mapArray.Size(); j++) // read map
			{
				if (mapArray[j].IsString())
				{
					char* mapRow = const_cast<char*>(mapArray[j].GetString());
					std::string mapRowString(mapRow);
					levelContainer.map.emplace_back(mapRowString);
				}
			}

			levelContainer.level = levelIdx;
			return true; // level found and map read
		}
	}

	return false;
}

void PacJSONParser::ParseJSONData(const std::string& levelDataFile)
{
	using namespace rapidjson;

	std::ifstream ifs(levelDataFile);
	if (!ifs.is_open()) {
		std::cerr << "Failed to open JSON file: " << levelDataFile << '\n';
		return;
	}

	IStreamWrapper isw(ifs);
	Document doc;
	doc.ParseStream(isw);

	if (doc.HasParseError()) {
		std::cerr << "Failed to parse JSON file: " << levelDataFile << '\n';
		return;
	}

	if (doc.IsObject()) {
		std::cerr << "Array expected." << '\n';
	}

	std::vector<PacData::PacLevelData> pacLevels{};

	if (!doc.HasMember("levels") || doc["levels"].IsArray())
	{
		const Value& levels = doc["levels"];
		for (SizeType i = 0; i < levels.Size(); i++)
		{
			PacData::PacLevelData pacLevel{};

			// level nr
			const Value& level = levels[i];
			if (level.HasMember("level") && level["level"].IsInt())
			{
				pacLevel.level = level["level"].GetInt();
			}

			// map
			if (doc.HasMember("map") && doc["map"].IsArray())
			{
				const Value& mapArray = doc["map"];
				for (SizeType j = 0; j < mapArray.Size(); j++)
				{
					if (mapArray[j].IsString())
					{
						char* mapRow = const_cast<char*>(mapArray[j].GetString());
						std::string mapRowString(mapRow);
						pacLevel.map.emplace_back(mapRowString);
					}
				}
			}

			pacLevels.emplace_back(pacLevel);
		}
	}
}
