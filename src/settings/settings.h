#pragma once

#include "../sdk/definitions/color.h"
#include <cstdint>
#include <memory>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include "settingentry.h"
#include "type.h"
#include "../features/binds/binds.h"

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

enum class BacktrackMode
{
	INVALID = -1,
	NONE, LAST_ONLY, ALL_RECORDS,
	MAX
};

enum class HealthMode
{
	INVALID = -1,
	NONE, TEXT, BAR, BOTH,
	MAX
};

enum class ESPTeamSelectionMode
{
	INVALID = -1,
	ENEMIES, TEAMMATES, BOTH,
	MAX,
};

enum class ESPConditionFlags
{
	Zoomed = 1 << 0,
	Bonked = 1 << 1,
	Ubered = 1 << 2,
	Jarated = 1 << 3,
};

namespace Settings
{
	struct SettingsAntiAim
	{
		Hotkey* warp_key = nullptr;
		Hotkey* warp_recharge_key = nullptr;
		Hotkey* warp_dt_key = nullptr;

		int spin_speed = 0;
		bool enabled = false;
		int pitch_mode = 0;
		int real_yaw_mode = 0;
		int fake_yaw_mode = 0;

		bool warp_enabled = false;
		int warp_speed = 1;

		bool fakelag_enabled = false;
		int fakelag_ticks = 1;
	};

	extern SettingsAntiAim AntiAim;
}

namespace Settings
{
	struct SettingsESP
	{
		int stencil = 0;
		int blur = 0;
		bool enabled = false;
		bool ignorecloaked = false;
		bool buildings = false;
		bool name = false;
		bool box = false;
		int health = 0;
		bool chams = 0;
		bool weapon = false;
		int team_selection = 0;
		int fconditions = 0;
	};

	extern SettingsESP ESP;
}

namespace Settings
{
	struct SettingsAimbot
	{
		bool enabled = false;
		float fov = 0.0f;
		Hotkey* key = nullptr;
		bool autoshoot = false;
		float max_sim_time = 0.0f;
		bool viewmodelaim = false;
		bool draw_fov_indicator = false;
		bool ignorecloaked = false;
		bool ignoreubered = false;
		bool ignorehoovy = false;
		bool ignorebonked = false;
		bool waitforcharge = false;
		bool hold_minigun_spin = false;
		int mode = 0;
		int melee = 0;
		int teamMode = 0;
		float smoothness = 0.0f;
		bool indicator = false;
	};

	extern SettingsAimbot Aimbot;
}

namespace Settings
{
	struct SettingsMisc
	{
		bool thirdperson = false;
		Hotkey* thirdperson_key = nullptr;
		bool customfov_enabled = false;
		float customfov = 90.0f;
		bool spectatorlist = false;
		bool backpack_expander = false;
		bool sv_pure_bypass = false;
		bool streamer_mode = false;
		bool bhop = false;
		bool accept_item_drop = false;
		bool playerlist = false;
		bool norecoil = false;
		float viewmodel_offset[3] = {0, 0, 0};
		float viewmodel_interp = 0.0f;
		int backtrack = 0;

		float thirdperson_offset[4] = {23.5, 11.5, 8.0f, 1.0f};
	};

	extern SettingsMisc Misc;
}

namespace Settings
{
	struct SettingsTriggerbot
	{
		Hotkey* key = nullptr;
		bool hitscan = false;
		int autobackstab = 0;
		int autoairblast = 0;
	};

	extern SettingsTriggerbot Trigger;
}

namespace Settings
{
	struct SettingsColors
	{
		Color red_team = {255, 0, 0, 255};
		Color blu_team = {0, 255, 255, 255};
		Color aimbot_target = {255, 255, 255, 255};
		Color weapon = {255, 255, 255, 255};
	};

	extern SettingsColors Colors;
}

namespace Settings
{
	struct SettingsRadar
	{
		int size = 120;
		int range = 3000;
		int icon_size = 10;
		bool enabled = false;
		bool players = false;
		bool buildings = false;
		bool objective = false;
		bool projectiles = false;
	};

	extern SettingsRadar Radar;
}

namespace Settings
{
	extern bool menu_open;
	inline constexpr SettingEntry m_entries[]
	{
		// aimbot
		//{"aimbot enabled", SettingType::BOOL, &Aimbot.enabled},
		{"aimbot fov", SettingType::FLOAT, &Aimbot.fov},
		//{"aimbot key", SettingType::STRING, Aimbot.key},
		{"aimbot autoshot", SettingType::BOOL, &Aimbot.autoshoot},
		{"aimbot max sim time", SettingType::FLOAT, &Aimbot.max_sim_time},
		{"aimbot viewmodel aim", SettingType::BOOL, &Aimbot.viewmodelaim},
		{"aimbot draw fov indicator", SettingType::BOOL, &Aimbot.draw_fov_indicator},
		{"aimbot ignore cloaked", SettingType::BOOL, &Aimbot.ignorecloaked},
		{"aimbot ignore ubered", SettingType::BOOL, &Aimbot.ignoreubered},
		{"aimbot ignore hoovy", SettingType::BOOL, &Aimbot.ignorehoovy},
		{"aimbot ignore bonked", SettingType::BOOL, &Aimbot.ignorebonked},
		{"aimbot melee", SettingType::INT, &Aimbot.melee},
		{"aimbot wait for charge", SettingType::BOOL, &Aimbot.waitforcharge},
		{"aimbot mode", SettingType::INT, &Aimbot.mode},
		{"aimbot smoothness", SettingType::FLOAT, &Aimbot.smoothness},
		{"aimbot team mode", SettingType::INT, &Aimbot.teamMode},
		{"aimbot hold minigun spin", SettingType::BOOL, &Aimbot.hold_minigun_spin},
		{"aimbot indicator", SettingType::BOOL, &Aimbot.indicator},

		// esp
		{"esp enabled", SettingType::BOOL, &ESP.enabled},
		{"esp ignore cloaked", SettingType::BOOL, &ESP.ignorecloaked},
		{"esp buildings", SettingType::BOOL, &ESP.buildings},
		{"esp name", SettingType::BOOL, &ESP.name},
		{"esp box", SettingType::BOOL, &ESP.box},
		{"esp health", SettingType::INT, &ESP.health},
		{"esp chams", SettingType::BOOL, &ESP.chams},
		{"esp stencil", SettingType::INT, &ESP.stencil},
		{"esp blur", SettingType::INT, &ESP.blur},
		{"esp weapon", SettingType::BOOL, &ESP.weapon},
		{"esp conditions", SettingType::INT, &ESP.fconditions},

		// misc
		{"misc thirdperson", SettingType::BOOL, &Misc.thirdperson},
		{"misc thirdperson key",SettingType::BIND, &Misc.thirdperson_key},
		{"misc customfov enabled", SettingType::BOOL, &Misc.customfov_enabled},
		{"misc customfov", SettingType::FLOAT, &Misc.customfov},
		{"misc spectatorlist", SettingType::BOOL, &Misc.spectatorlist},
		{"misc backpack_expander", SettingType::BOOL, &Misc.backpack_expander},
		{"misc sv pure bypass", SettingType::BOOL, &Misc.sv_pure_bypass},
		{"misc streamer mode", SettingType::BOOL, &Misc.streamer_mode},
		{"misc bhop",SettingType::BOOL,  &Misc.bhop},
		{"misc accept item drop", SettingType::BOOL, &Misc.accept_item_drop},
		{"misc playerlist", SettingType::BOOL, &Misc.playerlist},
		{"misc viewmodel offset x", SettingType::FLOAT, &Misc.viewmodel_offset[0]},
		{"misc viewmodel offset y", SettingType::FLOAT, &Misc.viewmodel_offset[1]},
		{"misc viewmodel offset z", SettingType::FLOAT, &Misc.viewmodel_offset[2]},
		{"misc viewmodel interp", SettingType::FLOAT, &Misc.viewmodel_interp},
		{"misc no recoil", SettingType::BOOL, &Misc.norecoil},

		//triggerbot
		//{"trigger enabled", SettingType::BOOL, &Trigger.enabled},
		{"trigger hitscan", SettingType::BOOL, &Trigger.hitscan},
		{"trigger autobackstab", SettingType::INT, &Trigger.autobackstab},
		//{"trigger key", SettingType::STRING, Trigger.key},
		{"trigger autoairblast", SettingType::INT, &Trigger.autoairblast},

		// colors
		/*{"colors red team r", SettingType::INT, Colors.red_team[0]},
		{"colors red team g", SettingType::INT, Colors.red_team[1]},
		{"colors red team b", SettingType::INT, Colors.red_team[2]},
		{"colors red team a", SettingType::INT, Colors.red_team[3]},
		{"colors blu team r", SettingType::INT, Colors.blu_team[0]},
		{"colors blu team g", SettingType::INT, Colors.blu_team[1]},
		{"colors blu team b", SettingType::INT, Colors.blu_team[2]},
		{"colors blu team a", SettingType::INT, Colors.blu_team[3]},
		{"colors aimbot target r", SettingType::INT, &Colors.aimbot_target[0]},
		{"colors aimbot target g", SettingType::INT, &Colors.aimbot_target[1]},
		{"colors aimbot target b", SettingType::INT, &Colors.aimbot_target[2]},
		{"colors aimbot target a", SettingType::INT, &Colors.aimbot_target[3]},
		{"colors weapon r", SettingType::INT, &Colors.weapon[0]},
		{"colors weapon g", SettingType::INT, &Colors.weapon[1]},
		{"colors weapon b", SettingType::INT, &Colors.weapon[2]},
		{"colors weapon a", SettingType::INT, &Colors.weapon[3]},*/

		// antiaim
		{"antiaim enabled", SettingType::BOOL, &AntiAim.enabled},
		{"antiaim pitch mode", SettingType::INT, &AntiAim.pitch_mode},
		{"antiaim real yaw mode", SettingType::INT, &AntiAim.real_yaw_mode},
		{"antiaim fake yaw mode", SettingType::INT, &AntiAim.fake_yaw_mode},
		{"antiaim spin speed", SettingType::FLOAT, &AntiAim.spin_speed},

		// warp
		{"warp enabled", SettingType::BOOL, &AntiAim.warp_enabled},
		{"warp speed", SettingType::INT, &AntiAim.warp_speed},
		//{"warp key", SettingType::STRING, AntiAim.warp_key},
		//{"warp recharge key", SettingType::STRING, AntiAim.warp_recharge_key},
		//{"doubletap key", SettingType::STRING, AntiAim.warp_dt_key},

		// fakelag
		{"fakelag enabled", SettingType::BOOL, &AntiAim.fakelag_enabled},
		{"fakelag ticks", SettingType::INT, &AntiAim.fakelag_ticks},

		// radar
		{"radar enabled", SettingType::BOOL, &Radar.enabled,},
		{"radar size", SettingType::INT, &Radar.size,},
		{"radar range", SettingType::INT, &Radar.range,},
		{"radar players", SettingType::BOOL, &Radar.players,},
		{"radar buildings", SettingType::BOOL, &Radar.buildings,},
		{"radar objective", SettingType::BOOL, &Radar.objective,},
		{"radar projectiles", SettingType::BOOL, &Radar.projectiles,},
		{"radar icon size", SettingType::INT, &Radar.icon_size,},
	};

	void Save(const std::string& fullPath);
	void Load(const std::string& fullPath);

	template<typename T>
	bool GetSetting(const std::string& key, T& out)
	{
		for (const auto& entry : m_entries)
		{
			if (entry.name != key)
				continue;

			out = *static_cast<T*>(entry.ptr);
			return true;
		}

		return false;
	}

	template<typename T>
	bool SetSetting(const std::string& key, const T& value)
	{
		for (auto& entry : m_entries)
		{
			if (entry.name != key)
				continue;

			*static_cast<T*>(entry.ptr) = value;
			return true;
		}

		return false;
	}
};

namespace Settings
{
	void InitBinds();
};