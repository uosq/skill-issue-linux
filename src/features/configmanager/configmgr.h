#pragma once

#include <string>
#include <vector>

#include "../feature.h"

class ConfigManager
{
public:
	int Save(const std::string& filePath);
	int Load(const std::string& filePath);
	int LoadDefault();
	bool Delete(const std::string& filePath);
	bool SetDefault(const std::string& configName);

	void RefreshConfigs();
	std::vector<std::string>& GetConfigs();
	const std::string& GetDefault() const;

private:
	std::vector<std::string> configs;
	std::string defaultConfig;
};

DECLARE_FEATURE(ConfigManager, configs)
