#include "configmgr.h"

#include <filesystem>
#include <string>
#include <vector>

#include "../../thirdparty/simpleini/SimpleIni.h"

#include "../config/config.h"
#include "../logs/logs.h"

#define CONFIG_FOLDER "./skill-issue/configs"

void ConfigManager::RefreshConfigs()
{
	configs.clear();

	if (!std::filesystem::exists(CONFIG_FOLDER))
		std::filesystem::create_directories(CONFIG_FOLDER);

	for (const auto &entry : std::filesystem::directory_iterator(CONFIG_FOLDER))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".ini")
			configs.push_back(entry.path().stem().string());
	}
}

int ConfigManager::Save(const std::string &fullPath)
{
	CSimpleIniA ini;
	ini.SetUnicode();

	for (IConfigValue* setting = IConfigValue::g_pHead; setting != nullptr; setting = setting->next)
	{
		std::string section_and_key = std::string(setting->GetName());

		std::string key = section_and_key;

		std::string str_value = "";
		auto live_ref = setting->GetRef();

		std::visit([&str_value](auto&& arg)
		{
			using T = std::decay_t<decltype(arg)>;

			if constexpr (std::is_same_v<T, int*>)
				str_value = std::to_string(*arg);
			else if constexpr (std::is_same_v<T, float*>)
				str_value = std::to_string(*arg);
			else if constexpr (std::is_same_v<T, bool*>)
				str_value = *arg ? "true" : "false";
			else if constexpr (std::is_same_v<T, Color*>)
			{
				str_value = std::to_string(arg->r()) + " " + std::to_string(arg->g()) + " " +
					    std::to_string(arg->b()) + " " + std::to_string(arg->a());
			}
			else if constexpr (std::is_same_v<T, Hotkey*>)
				str_value = std::to_string(arg->m_iKey) + " " + std::to_string((int)arg->m_iMode);
			else if constexpr (std::is_same_v<T, std::array<float, 3>*>)
				str_value = std::to_string((*arg)[0]) + " " + std::to_string((*arg)[1]) + " " + std::to_string((*arg)[2]);
			else if constexpr (std::is_same_v<T, std::array<float, 4>*>)
			{
				str_value = std::to_string((*arg)[0]) + " " + std::to_string((*arg)[1]) + " " +
					    std::to_string((*arg)[2]) + " " + std::to_string((*arg)[3]);
			}
		}, live_ref);

		ini.SetValue("Settings", key.c_str(), str_value.c_str());
	}

	return ini.SaveFile(fullPath.c_str());
}

int ConfigManager::Load(const std::string &fullPath)
{
	CSimpleIniA ini;
	ini.SetUnicode();

	SI_Error rc = ini.LoadFile(fullPath.c_str());
	if (rc < 0)
	{
		features::logs.Error("Failed to load settings file");
		return rc;
	}

	for (IConfigValue* setting = IConfigValue::g_pHead; setting != nullptr; setting = setting->next)
	{
		std::string key = std::string(setting->GetName());
		const char* val = ini.GetValue("Settings", key.c_str());

		if (val == nullptr)
		{
			features::logs.Warn("Couldn't find setting: " + key);
			continue;
		}

		std::stringstream ss(val);
		auto live_ref = setting->GetRef();

		std::visit([&ss](auto&& arg)
		{
			using T = std::decay_t<decltype(arg)>;

			if constexpr (std::is_same_v<T, int*> || std::is_same_v<T, uint32_t*>)
				ss >> *arg;
			else if constexpr (std::is_same_v<T, float*>)
				ss >> *arg;
			else if constexpr (std::is_same_v<T, bool*>)
			{
				std::string str;
				ss >> str;
				*arg = (str == "true" || str == "1");
			}
			else if constexpr (std::is_same_v<T, Color*>)
			{
				int r, g, b, a;
				if (ss >> r >> g >> b >> a)
					*arg = Color{r, g, b, a};
			}
			else if constexpr (std::is_same_v<T, Hotkey*>)
			{
				int key_code, mode_code;
				if (ss >> key_code >> mode_code)
				{
					arg->m_iKey = key_code;
					arg->m_iMode = static_cast<HotkeyMode>(mode_code);
					arg->m_iType = (key_code > 0) ? InputType::VirtualKey : InputType::None;
				}
			}
			else if constexpr (std::is_same_v<T, std::array<float, 3>*>)
				ss >> (*arg)[0] >> (*arg)[1] >> (*arg)[2];
			else if constexpr (std::is_same_v<T, std::array<float, 4>*>)
				ss >> (*arg)[0] >> (*arg)[1] >> (*arg)[2] >> (*arg)[3];
		}, live_ref);
	}

	return SI_OK;
}

bool ConfigManager::Delete(const std::string &filePath)
{
	return std::filesystem::remove(filePath);
}

std::vector<std::string>& ConfigManager::GetConfigs()
{
	return configs;
}