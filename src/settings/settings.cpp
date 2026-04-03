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

void Settings::Load(const std::string &fullPath)
{
	CSimpleIniA ini;
	ini.SetUnicode();

	SI_Error rc = ini.LoadFile(fullPath.c_str());

	if (rc < 0)
		return Logs::Error("Failed to load settings file");

	for (auto &setting : m_entries)
	{
		const char *val = ini.GetValue("Settings", std::string(setting.name).c_str());
		if (val)
			setting.load(val);
	}
}

void Settings::Save(const std::string &fullPath)
{
	CSimpleIniA ini;

	for (const auto &setting : m_entries)
		ini.SetValue("Settings", std::string(setting.name).c_str(), setting.get().c_str());

	ini.SaveFile(fullPath.c_str());
}

void Settings::InitBinds()
{
	Aimbot.key		  = gBinds.RegisterHotkey("aimbot key");
	AntiAim.warp_key	  = gBinds.RegisterHotkey("warp key");
	AntiAim.warp_recharge_key = gBinds.RegisterHotkey("warp recharge key");
	Misc.thirdperson_key	  = gBinds.RegisterHotkey("thirdperson key");
	Trigger.key		  = gBinds.RegisterHotkey("trigger key");
}