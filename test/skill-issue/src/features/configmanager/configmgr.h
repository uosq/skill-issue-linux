#pragma once

#include <string>
#include <vector>

#include "../feature.h"

class ConfigManager
{
public:
	void Save(const std::string& filePath);
	void Load(const std::string& filePath);
	bool Delete(const std::string& filePath);

	void RefreshConfigs();
	std::vector<std::string>& GetConfigs();

private:
	std::vector<std::string> configs;
};

DECLARE_FEATURE(ConfigManager, configs)