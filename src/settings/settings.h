#pragma once

#include "../features/binds/binds.h"
#include "../sdk/definitions/color.h"
#include "settingentry.h"
#include "type.h"
#include <cstdint>
#include <memory>
#include <sstream>
#include <string>
#include <unistd.h>
#include <unordered_map>

enum class PitchMode
{
	NONE = 0,
	UP,
	DOWN,
	FAKEUP,
	FAKEDOWN,
	RANDOM,
};

enum class YawMode
{
	NONE = 0,
	LEFT,
	RIGHT,
	BACK,
	FORWARD,
	SPIN_LEFT,
	SPIN_RIGHT,
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

enum class GenericMode
{
	NONE = 0,
	LEGIT,
	RAGE
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
	NONE,
	LAST_ONLY,
	ALL_RECORDS,
	MAX
};

enum class HealthMode
{
	INVALID = -1,
	NONE,
	TEXT,
	BAR,
	BOTH,
	MAX
};

enum class ESPTeamSelectionMode
{
	INVALID = -1,
	ENEMIES,
	TEAMMATES,
	BOTH,
	MAX,
};

enum class ESPConditionFlags
{
	Zoomed	= 1 << 0,
	Bonked	= 1 << 1,
	Ubered	= 1 << 2,
	Jarated = 1 << 3,
};

enum class AimbotIndicatorStyle
{
	NONE = 0,
	CIRCLE,
	SQUARE,
	TRIANGLE,
};

enum class ESPFont
{
	INVALID = -1,
	TF2BUILD,
	ARIAL,
	COUNT,
};

namespace Settings
{
	struct SettingsAntiAim
	{
		Hotkey *warp_key	  = nullptr;
		Hotkey *warp_recharge_key = nullptr;

		int spin_speed		  = 0;
		bool enabled		  = false;
		int pitch_mode		  = 0;
		int real_yaw_mode	  = 0;
		int fake_yaw_mode	  = 0;

		bool warp_enabled	  = false;
		int warp_speed		  = 1;

		bool fakelag_enabled	  = false;
		int fakelag_ticks	  = 1;
	};

	extern SettingsAntiAim AntiAim;
} // namespace Settings

namespace Settings
{
	struct SettingsESP
	{
		int stencil	   = 0;
		int blur	   = 0;
		bool enabled	   = false;
		bool ignorecloaked = false;
		bool buildings	   = false;
		bool name	   = false;
		bool box	   = false;
		int health	   = 0;
		bool chams	   = false;
		float chams_alpha  = 0.5f;
		bool weapon	   = false;
		int team_selection = 0;
		int fconditions	   = 0;
		int font	   = 0;
	};

	extern SettingsESP ESP;
} // namespace Settings

namespace Settings
{
	struct SettingsAimbot
	{
		bool enabled		= false;
		float fov		= 0.0f;
		Hotkey *key		= nullptr;
		bool autoshoot		= false;
		float max_sim_time	= 0.0f;
		bool viewmodelaim	= false;
		bool draw_fov_indicator = false;
		bool ignorecloaked	= false;
		bool ignoreubered	= false;
		bool ignorehoovy	= false;
		bool ignorebonked	= false;
		bool waitforcharge	= false;
		bool hold_minigun_spin	= false;
		int mode		= 0;
		int melee		= 0;
		int teamMode		= 0;
		float smoothness	= 0.0f;
		int indicator		= 0;
		bool path		= false;
	};

	extern SettingsAimbot Aimbot;
} // namespace Settings

namespace Settings
{
	struct SettingsMisc
	{
		bool thirdperson	    = false;
		Hotkey *thirdperson_key	    = nullptr;
		bool customfov_enabled	    = false;
		float customfov		    = 90.0f;
		bool spectatorlist	    = false;
		bool backpack_expander	    = false;
		bool sv_pure_bypass	    = false;
		bool streamer_mode	    = false;
		bool bhop		    = false;
		bool autostrafe		    = false;
		bool accept_item_drop	    = false;
		bool playerlist		    = false;
		bool norecoil		    = false;
		float viewmodel_offset[3]   = {0, 0, 0};
		float viewmodel_interp	    = 0.0f;
		bool no_viewmodel_bob	    = false;
		int backtrack		    = 0;
		float thirdperson_offset[4] = {23.5, 11.5, 8.0f, 1.0f};
		bool no_engine_sleep	    = false;
		bool no_scope_overlay	    = false;
		bool no_zoom		    = false;
		//bool no_survey = false;
	};

	extern SettingsMisc Misc;
} // namespace Settings

namespace Settings
{
	struct SettingsTriggerbot
	{
		Hotkey *key	 = nullptr;
		bool hitscan	 = false;
		int autobackstab = 0;
		int autoairblast = 0;
		int autosticky	 = 0;
	};

	extern SettingsTriggerbot Trigger;
} // namespace Settings

namespace Settings
{
	struct SettingsColors
	{
		Color red_team	    = {255, 0, 0, 255};
		Color blu_team	    = {0, 255, 255, 255};
		Color aimbot_target = {255, 255, 255, 255};
		Color weapon	    = {255, 255, 255, 255};
	};

	extern SettingsColors Colors;
} // namespace Settings

namespace Settings
{
	struct SettingsRadar
	{
		int size	 = 120;
		int range	 = 3000;
		int icon_size	 = 10;
		bool enabled	 = false;
		bool players	 = false;
		bool buildings	 = false;
		bool objective	 = false;
		bool projectiles = false;
	};

	extern SettingsRadar Radar;
} // namespace Settings

#define CONFIG_INT(name, var)                                                                                          \
	{name,                                                                                                         \
	 SettingType::INT,                                                                                             \
	 [](std::ofstream &f) { f << (var); },                                                                         \
	 [](const std::string &v) { var = std::stoi(v); },                                                             \
	 []() { return std::to_string(var); },                                                                         \
	 [](const std::string &v) { var = std::stoi(v); }}

#define CONFIG_BOOL(name, var)                                                                                         \
	{name,                                                                                                         \
	 SettingType::BOOL,                                                                                            \
	 [](std::ofstream &f) { f << ((var) ? 1 : 0); },                                                               \
	 [](const std::string &v) { var = std::stoi(v) != 0; },                                                        \
	 []() { return std::to_string(var); },                                                                         \
	 [](const std::string &v) { var = std::stoi(v) != 0; }}

#define CONFIG_FLOAT(name, var)                                                                                        \
	{name,                                                                                                         \
	 SettingType::FLOAT,                                                                                           \
	 [](std::ofstream &f) { f << (var); },                                                                         \
	 [](const std::string &v) { var = std::stof(v); },                                                             \
	 []() { return std::to_string(var); },                                                                         \
	 [](const std::string &v) { var = std::stof(v); }}

#define CONFIG_STRING(name, var)                                                                                       \
	{name,                                                                                                         \
	 SettingType::STRING,                                                                                          \
	 [](std::ofstream &f) { f << (var); },                                                                         \
	 [](const std::string &v) { var = v; },                                                                        \
	 []() { return std::string(var); },                                                                            \
	 [](const std::string &v) { var = v; }}

#define CONFIG_ENUM(name, var, EnumType)                                                                               \
	{name,                                                                                                         \
	 SettingType::INT,                                                                                             \
	 [](std::ofstream &f) { f << static_cast<int>(var); },                                                         \
	 [](const std::string &v) { var = static_cast<EnumType>(std::stoi(v)); },                                      \
	 []() { return std::to_string(static_cast<int>(var)); },                                                       \
	 [](const std::string &v) { var = static_cast<EnumType>(std::stoi(v)); }}

#define CONFIG_KEY(name, var)                                                                                          \
	CONFIG_INT(name " key", var->m_iKey), CONFIG_ENUM(name " mode", var->m_iMode, HotkeyMode),                     \
	    CONFIG_ENUM(name " type", var->m_iType, InputType)

#define CONFIG_FLOAT3(name, var)                                                                                       \
	CONFIG_FLOAT(name " x", var[0]), CONFIG_FLOAT(name " y", var[1]), CONFIG_FLOAT(name " z", var[2])

#define CONFIG_FLOAT4(name, var) CONFIG_FLOAT3(name, var), CONFIG_FLOAT(name, var[3])

namespace Settings
{
	extern bool menu_open;
	inline SettingEntry m_entries[]{
	    // aimbot
	    CONFIG_FLOAT("aimbot fov", Aimbot.fov),
	    CONFIG_BOOL("aimbot autoshot", Aimbot.autoshoot),
	    CONFIG_FLOAT("aimbot max sim time", Aimbot.max_sim_time),
	    CONFIG_BOOL("aimbot viewmodel aim", Aimbot.viewmodelaim),
	    CONFIG_BOOL("aimbot draw fov indicator", Aimbot.draw_fov_indicator),
	    CONFIG_BOOL("aimbot ignore cloaked", Aimbot.ignorecloaked),
	    CONFIG_BOOL("aimbot ignore ubered", Aimbot.ignoreubered),
	    CONFIG_BOOL("aimbot ignore hoovy", Aimbot.ignorehoovy),
	    CONFIG_BOOL("aimbot ignore bonked", Aimbot.ignorebonked),
	    CONFIG_INT("aimbot melee", Aimbot.melee),
	    CONFIG_BOOL("aimbot wait for charge", Aimbot.waitforcharge),
	    CONFIG_INT("aimbot mode", Aimbot.mode),
	    CONFIG_FLOAT("aimbot smoothness", Aimbot.smoothness),
	    CONFIG_INT("aimbot team mode", Aimbot.teamMode),
	    CONFIG_BOOL("aimbot hold minigun spin", Aimbot.hold_minigun_spin),
	    CONFIG_BOOL("aimbot indicator", Aimbot.indicator),
	    CONFIG_KEY("aimbot key", Aimbot.key),
	    CONFIG_INT("aimbot path", Aimbot.path),
	    CONFIG_INT("aimbot indicator", Aimbot.indicator),

	    // esp
	    CONFIG_BOOL("esp enabled", ESP.enabled),
	    CONFIG_BOOL("esp ignore cloaked", ESP.ignorecloaked),
	    CONFIG_BOOL("esp buildings", ESP.buildings),
	    CONFIG_BOOL("esp name", ESP.name),
	    CONFIG_BOOL("esp box", ESP.box),
	    CONFIG_INT("esp health", ESP.health),
	    CONFIG_BOOL("esp chams", ESP.chams),
	    CONFIG_INT("esp stencil", ESP.stencil),
	    CONFIG_INT("esp blur", ESP.blur),
	    CONFIG_BOOL("esp weapon", ESP.weapon),
	    CONFIG_INT("esp conditions", ESP.fconditions),
	    CONFIG_INT("esp team", ESP.team_selection),
	    CONFIG_INT("esp font", ESP.font),

	    // misc
	    CONFIG_KEY("misc thirdperson key", Misc.thirdperson_key),
	    CONFIG_FLOAT3("misc thirdperson offset", Misc.thirdperson_offset),
	    CONFIG_BOOL("misc customfov enabled", Misc.customfov_enabled),
	    CONFIG_FLOAT("misc customfov", Misc.customfov),
	    CONFIG_BOOL("misc spectatorlist", Misc.spectatorlist),
	    CONFIG_BOOL("misc backpack_expander", Misc.backpack_expander),
	    CONFIG_BOOL("misc sv pure bypass", Misc.sv_pure_bypass),
	    CONFIG_BOOL("misc streamer mode", Misc.streamer_mode),
	    CONFIG_BOOL("misc bhop", Misc.bhop),
	    CONFIG_BOOL("misc autostrafe", Misc.autostrafe),
	    CONFIG_BOOL("misc accept item drop", Misc.accept_item_drop),
	    CONFIG_BOOL("misc playerlist", Misc.playerlist),
	    CONFIG_FLOAT3("misc viewmodel offset", Misc.viewmodel_offset),
	    CONFIG_FLOAT("misc viewmodel interp", Misc.viewmodel_interp),
	    CONFIG_BOOL("misc no recoil", Misc.norecoil),
	    CONFIG_INT("misc backtrack", Misc.backtrack),
	    CONFIG_BOOL("misc no engine sleep", Misc.no_engine_sleep),
	    CONFIG_BOOL("no scope overlay", Misc.no_scope_overlay),
	    CONFIG_BOOL("no zoom", Misc.no_zoom),
	    CONFIG_BOOL("no viewmodel bob", Misc.no_viewmodel_bob),
	    //CONFIG_BOOL("no survey", Misc.no_survey),

	    //triggerbot
	    CONFIG_KEY("trigger key", Trigger.key),
	    CONFIG_BOOL("trigger hitscan", Trigger.hitscan),
	    CONFIG_INT("trigger autobackstab", Trigger.autobackstab),
	    CONFIG_INT("trigger autoairblast", Trigger.autoairblast),

	    // colors
	    CONFIG_FLOAT4("colors red team", Colors.red_team),
	    CONFIG_FLOAT4("colors blu team", Colors.blu_team),
	    CONFIG_FLOAT4("colors aimbot target", Colors.aimbot_target),
	    CONFIG_FLOAT4("colors weapon", Colors.weapon),

	    // antiaim
	    CONFIG_BOOL("antiaim enabled", AntiAim.enabled),
	    CONFIG_INT("antiaim pitch mode", AntiAim.pitch_mode),
	    CONFIG_INT("antiaim real yaw mode", AntiAim.real_yaw_mode),
	    CONFIG_INT("antiaim fake yaw mode", AntiAim.fake_yaw_mode),
	    CONFIG_FLOAT("antiaim spin speed", AntiAim.spin_speed),

	    // warp
	    CONFIG_BOOL("warp enabled", AntiAim.warp_enabled),
	    CONFIG_INT("warp speed", AntiAim.warp_speed),
	    CONFIG_KEY("warp key", AntiAim.warp_key),
	    CONFIG_KEY("warp recharge key", AntiAim.warp_recharge_key),

	    // fakelag
	    CONFIG_BOOL("fakelag enabled", AntiAim.fakelag_enabled),
	    CONFIG_INT("fakelag ticks", AntiAim.fakelag_ticks),

	    // radar
	    CONFIG_BOOL("radar enabled", Radar.enabled),
	    CONFIG_INT("radar size", Radar.size),
	    CONFIG_INT("radar range", Radar.range),
	    CONFIG_BOOL("radar players", Radar.players),
	    CONFIG_BOOL("radar buildings", Radar.buildings),
	    CONFIG_BOOL("radar objective", Radar.objective),
	    CONFIG_BOOL("radar projectiles", Radar.projectiles),
	    CONFIG_INT("radar icon size", Radar.icon_size),
	};

	void Save(const std::string &fullPath);
	void Load(const std::string &fullPath);

	template <typename T> bool GetSetting(const std::string &key, T &out)
	{
		for (const auto &entry : m_entries)
		{
			if (entry.name != key)
				continue;

			std::stringstream ss(entry.get());
			ss >> out;
			return true;
		}

		return false;
	}

	template <typename T> bool SetSetting(const std::string &key, const T &value)
	{
		for (auto &entry : m_entries)
		{
			if (entry.name != key)
				continue;

			// this is next level stupid
			// why do i have to do this?
			// why the fuck std::to_string can't handle a fucking string?
			// fuck you C++
			if constexpr (std::is_same_v<T, std::string>)
				entry.set(value);
			else
				entry.set(std::to_string(value));
			return true;
		}

		return false;
	}
}; // namespace Settings

#undef CONFIG_INT
#undef CONFIG_FLOAT
#undef CONFIG_BOOL
#undef CONFIG_ENUM
#undef CONFIG_STRING

namespace Settings
{
	void InitBinds();
};
