#pragma once

#include "../sdk/definitions/color.h"
#include <cstdint>
#include <memory>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include "settingentry.h"
#include "type.h"

enum class PitchMode
{
	NONE = 0,
	UP, DOWN,
	FAKEUP, FAKEDOWN,
	RANDOM,
};

enum class YawMode
{
	NONE = 0,
	LEFT, RIGHT,
	BACK, FORWARD,
	SPIN_LEFT, SPIN_RIGHT,
	JITTER,
};

enum class MeleeMode
{
	NONE = 0,
	LEGIT,
	RAGE
};

enum class AimbotMode
{
	INVALID = -1,
	PLAIN,
	SMOOTH,
	ASSISTANCE,
	SILENT,
	MAX
};

enum class AutoBackstabMode
{
	NONE = 0,
	LEGIT, RAGE
};

enum class TeamMode
{
	INVALID = -1,
	ONLYENEMY,
	ONLYTEAMMATE,
	BOTH,
	MAX
};

namespace Settings
{
	namespace AntiAim
	{
		extern bool enabled;
		extern int pitch_mode;
		extern int real_yaw_mode;
		extern int fake_yaw_mode;
		extern float spin_speed;

		extern bool warp_enabled;
		extern std::string warp_key;
		extern std::string warp_recharge_key;
		extern std::string warp_dt_key;
		extern int warp_speed;
	}
}

namespace Settings
{
	namespace ESP
	{
		extern bool enabled;
		extern bool ignorecloaked;
		extern bool buildings;
		extern bool name;
		extern bool box;
		extern bool healthbar;
		extern bool chams;
		extern int stencil;
		extern int blur;
		extern bool weapon;
	}
}

namespace Settings
{
	namespace Aimbot
	{
		extern bool enabled;
		extern float fov;
		extern std::string key;
		extern bool autoshoot;
		extern float max_sim_time;
		extern bool viewmodelaim;
		extern bool draw_fov_indicator;
		extern bool ignorecloaked;
		extern bool ignoreubered;
		extern bool ignorehoovy;
		extern bool ignorebonked;
		extern bool waitforcharge;
		extern bool hold_minigun_spin;
		extern int mode;
		extern int melee;
		extern int teamMode;
		extern float smoothness;
	}
}

namespace Settings
{
	namespace Misc
	{
		extern bool thirdperson;
		extern std::string thirdperson_key;
		extern bool customfov_enabled;
		extern float customfov;
		extern bool spectatorlist;
		extern bool backpack_expander;
		extern bool sv_pure_bypass;
		extern bool streamer_mode;
		extern bool bhop;
		extern bool accept_item_drop;
		extern bool playerlist;
		extern bool norecoil;
		extern float viewmodel_offset[3];
		extern float viewmodel_interp;
	}
}

namespace Settings
{
	namespace Triggerbot
	{
		extern bool enabled;
		extern bool hitscan;
		extern int autobackstab;
		extern std::string key;
		extern int autoairblast;
	}
}

namespace Settings
{
	namespace Colors
	{
		extern Color red_team;
		extern Color blu_team;
		extern Color aimbot_target;
		extern Color weapon;
	}
}

namespace Settings
{
	namespace Radar
	{
		extern int size;
		extern int range;
		extern int icon_size;
		extern bool enabled;
		extern bool players;
		extern bool buildings;
		extern bool objective;
		extern bool projectiles;
	}
}

namespace Settings
{
	extern bool menu_open;
	inline std::unordered_map<std::string, std::unique_ptr<ISettingEntry>>& GetOptionMap()
	{
		static std::unordered_map<std::string, std::unique_ptr<ISettingEntry>> map;
		return map;
	}

	void RegisterOptions(void);
	void SaveAll(const std::string& fullPath);
	void LoadAll(const std::string& fullPath);

	template<typename T>
	bool GetSetting(const std::string& key, T& out)
	{
		auto it = GetOptionMap().find(key);
		if (it == GetOptionMap().end())
			return false;

		auto* option = dynamic_cast<SettingEntry<T>*>(it->second.get());
		if (option == nullptr)
			return false;

		out = option->GetValue();
		return true;
	}

	template<typename T>
	bool SetSetting(const std::string& key, const T& value)
	{
		auto it = GetOptionMap().find(key);
		if (it == GetOptionMap().end())
			return false;

		auto* option = dynamic_cast<SettingEntry<T>*>(it->second.get());
		if (option == nullptr)
			return false;

		option->SetValue(value);
		return true;
	}
};