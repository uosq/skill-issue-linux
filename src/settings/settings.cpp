#include "settings.h"
#include <cstring>
#include <fstream>
#include "../features/logs/logs.h"

bool Settings::menu_open = false;

namespace Settings
{
	SettingsAimbot Aimbot = {};
	SettingsAntiAim AntiAim = {};
	SettingsColors Colors = {};
	SettingsESP ESP = {};
	SettingsMisc Misc = {};
	SettingsRadar Radar = {};
	SettingsTriggerbot Trigger = {};
}

void Settings::Load(const std::string& fullPath)
{
	std::ifstream file(fullPath);
	if (!file.is_open())
		return Logs::Error("Tried loading. File is not open");
	
	std::string line;
	
	while (std::getline(file, line))
	{
		if (line.empty())
			continue;
	
		const auto separatorPos = line.find('=');
		if (separatorPos == std::string::npos)
			continue;
	
		std::string key = line.substr(0, separatorPos);
		std::string value = line.substr(separatorPos + 1);
	
		for (auto& setting : m_entries)
		{
			if (setting.name != key || setting.ptr == nullptr)
				continue;
	
			switch (setting.type)
			{
				case SettingType::BOOL:
				{
					*static_cast<bool*>(setting.ptr) = (value == "1" || value == "true");
					break;
				}
		
				case SettingType::INT:
				{
					*static_cast<int*>(setting.ptr) = std::stoi(value);
					break;
				}
		
				case SettingType::FLOAT:
				{
					*static_cast<float*>(setting.ptr) = std::stof(value);
					break;
				}
		
				case SettingType::STRING:
				{
					char* buffer = static_cast<char*>(setting.ptr);
					strncpy(buffer, value.c_str(), 15);
					buffer[15] = '\0';
					break;
				}
			}

			break;
		}
	}

	file.close();
}

void Settings::Save(const std::string &fullPath)
{
	std::ofstream file(fullPath, std::ios::app);
	if (!file.is_open())
		return Logs::Error("Tried saving. File is not open");

	for (const auto& setting : m_entries)
        {
		if (setting.ptr == nullptr)
			continue;

		file << setting.name << "=";

		switch(setting.type)
		{
                case SettingType::BOOL:
			file << (*static_cast<bool*>(setting.ptr) ? 1 : 0);
			break;
                case SettingType::INT:
			file << *static_cast<int*>(setting.ptr);
			break;
                case SettingType::FLOAT:
			file << *static_cast<float*>(setting.ptr);
			break;
                case SettingType::STRING:
			file << static_cast<char*>(setting.ptr);
                	break;
                }

		file << "\n";
        }

	file.close();
}