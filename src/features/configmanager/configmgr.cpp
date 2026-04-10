#include "configmgr.h"

#include <filesystem>
#include <string>
#include <vector>

#include "../../settings/settings.h"

#define CONFIG_FOLDER "./skill-issue/configs"

static std::vector<std::string> s_vConfigs;

void ConfigManager::RefreshConfigs()
{
	s_vConfigs.clear();

	if (!std::filesystem::exists(CONFIG_FOLDER))
		std::filesystem::create_directories(CONFIG_FOLDER);

	for (const auto &entry : std::filesystem::directory_iterator(CONFIG_FOLDER))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".ini")
			s_vConfigs.push_back(entry.path().stem().string());
	}
}

void ConfigManager::Save(const std::string& filePath)
{
	Settings::Save(filePath);
}

void ConfigManager::Load(const std::string &filePath)
{
	Settings::Load(filePath);
}

bool ConfigManager::Delete(const std::string &filePath)
{
	return std::filesystem::remove(filePath);
}

std::vector<std::string>& ConfigManager::GetConfigs()
{
	return s_vConfigs;
}