#pragma once

#include <string>
#include <vector>

namespace ConfigManager
{
	void Save(const std::string& filePath);
	void Load(const std::string& filePath);
	bool Delete(const std::string& filePath);

	void RefreshConfigs();
	std::vector<std::string>& GetConfigs();
}