#pragma once

#include <string>
#include <vector>

#include "../feature.h"

class ConfigManager
{
public:
	int Save(const std::string& filePath);
	int Load(const std::string& filePath);
	bool Delete(const std::string& filePath);

	void RefreshConfigs();
	std::vector<std::string>& GetConfigs();

private:
	std::vector<std::string> configs;
};

DECLARE_FEATURE(ConfigManager, configs)