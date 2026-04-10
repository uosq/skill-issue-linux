#include "settings.h"

#include "../features/logs/logs.h"
#include "../simpleini/SimpleIni.h"

bool Settings::menu_open = false;

namespace Settings
{
	SettingsAimbot Aimbot{};
	SettingsAntiAim AntiAim{};
	SettingsColors Colors{};
	SettingsESP ESP{};
	SettingsMisc Misc{};
	SettingsRadar Radar{};
	SettingsTriggerbot Trigger{};
} // namespace Settings

int Settings::Load(const std::string &fullPath)
{
	CSimpleIniA ini;
	ini.SetUnicode();

	SI_Error rc = ini.LoadFile(fullPath.c_str());

	if (rc < 0)
	{
		Logs::Error("Failed to load settings file");
		return rc;
	}

	for (auto &setting : m_entries)
	{
		std::string strName = std::string(setting.name);
		const char *val = ini.GetValue("Settings", strName.c_str());

		if (val == NULL)
		{
			Logs::Warn("Couldn't load setting \"" + strName + "\" with value \"" + setting.get() + "\"");
			continue;
		}

		setting.load(val);
	}

	return SI_OK;
}

int Settings::Save(const std::string &fullPath)
{
	CSimpleIniA ini;

	for (const auto &setting : m_entries)
		ini.SetValue("Settings", std::string(setting.name).c_str(), setting.get().c_str());

	return ini.SaveFile(fullPath.c_str());
}

void Settings::InitBinds()
{
	Aimbot.key		  = gBinds.RegisterHotkey("aimbot key");
	AntiAim.warp_key	  = gBinds.RegisterHotkey("warp key");
	AntiAim.warp_recharge_key = gBinds.RegisterHotkey("warp recharge key");
	Misc.thirdperson_key	  = gBinds.RegisterHotkey("thirdperson key");
	Trigger.key		  = gBinds.RegisterHotkey("trigger key");
}