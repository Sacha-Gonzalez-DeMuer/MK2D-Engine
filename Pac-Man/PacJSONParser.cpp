#include <fstream>
#include <iostream>
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
#include "PacJSONParser.h"

PacJSONParser::PacJSONParser(const std::string& dataFile)
	: m_dataFile{dataFile}
{}

bool PacJSONParser::LoadLevel(int levelIdx, PacData::PacLevelData& levelContainer)
{
	using namespace rapidjson;

	std::ifstream ifs(m_dataFile);
	if (!ifs.is_open()) {
		std::cerr << "Failed to open JSON file: " << m_dataFile << '\n';
		return false;
	}

	IStreamWrapper isw(ifs);
	Document doc;

	doc.ParseStream(isw);
	if (doc.HasParseError()) {
		std::cerr << "Failed to parse JSON file: " << m_dataFile << '\n';
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
			return true; // level found and map read
		}
	}

	return false;
}

bool PacJSONParser::LoadGameData(PacData::PacGameData& gameDataContainer)
{
	using namespace rapidjson;

	std::ifstream ifs(m_dataFile);
	if (!ifs.is_open()) {
		std::cerr << "Failed to open JSON file: " << m_dataFile << '\n';
		return false;
	}

	IStreamWrapper isw(ifs);
	Document doc;

	doc.ParseStream(isw);
	if (doc.HasParseError()) {
		std::cerr << "Failed to parse JSON file: " << m_dataFile << '\n';
		return false;
	}


	const rapidjson::Value& levels = doc["levels"];

	for (rapidjson::SizeType i = 0; i < levels.Size(); ++i)
	{
		const rapidjson::Value& level = levels[i];
		const rapidjson::Value& map = level["map"];
		PacData::PacGameData::PacMap pacMap;

		for (rapidjson::SizeType j = 0; j < map.Size(); ++j)
		{
			const std::string& row = map[j].GetString();
			pacMap.push_back(row);
		}

		gameDataContainer.maps.push_back(pacMap);
	}

	return true;
}
