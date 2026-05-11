#include "settings.h"

#include "../features/logs/logs.h"
#include "../simpleini/SimpleIni.h"

bool Settings::menu_open = false;

int Settings::Load(const std::string &fullPath)
{
	CSimpleIniA ini;
	ini.SetUnicode();

	SI_Error rc = ini.LoadFile(fullPath.c_str());

	if (rc < 0)
	{
		features::logs.Error("Failed to load settings file");
		return rc;
	}

	for (auto &setting : m_entries)
	{
		std::string strName = std::string(setting.name);
		const char *val = ini.GetValue("Settings", strName.c_str());

		if (val == NULL)
		{
			features::logs.Warn("Couldn't load setting \"" + strName + "\" with value \"" + setting.get() + "\"");
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
	Config.aimbot.key		= features::binds.RegisterHotkey("aimbot key");
	Config.warp.key	 		= features::binds.RegisterHotkey("warp key");
	Config.warp.recharge_key	= features::binds.RegisterHotkey("warp recharge key");
	Config.misc.thirdperson_key	= features::binds.RegisterHotkey("thirdperson key");
	Config.trigger.key		= features::binds.RegisterHotkey("trigger key");
}