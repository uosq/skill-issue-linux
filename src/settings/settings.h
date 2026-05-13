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

struct WarpConfig
{
	Hotkey *key		= nullptr;
	Hotkey *recharge_key	= nullptr;

	union
	{
		uint8_t raw = 0;

		struct
		{
			uint8_t enabled : 1;
			uint8_t speed : 5;

			uint8_t _reserved : 2; // pading
		};
	} packed;
};

struct ESPConfig
{
	union
	{
		uint8_t raw = 0;

		struct
		{
			uint8_t enabled : 1;
			uint8_t ignorecloaked : 1;
			uint8_t buildings : 1;
			uint8_t name : 1;
			uint8_t box : 1;
			uint8_t weapon : 1;
			uint8_t class_name : 1;
			uint8_t medkit : 1;
			uint8_t ammopack : 1;

			uint8_t _reserved : 1; // padding
		};
	} packed;

	union
	{
		uint8_t raw = 0;

		struct
		{
			uint8_t selected : 1; // arial or tf2 build
			uint8_t size : 6;

			uint8_t _reserved : 1; // padding
		};
	} font;

	union
	{
		uint8_t raw = 0;

		struct
		{
			uint8_t zoomed : 1;
			uint8_t bonked : 1;
			uint8_t ubered : 1;
			uint8_t jarated : 1;

			uint8_t _reserved : 4; // padding
		};
	} conditions;

	union
	{
		uint32_t raw = 0;
		struct
		{
			uint32_t healthbar  : 3;
			uint32_t weaponname : 3;
			uint32_t classname  : 3;
			uint32_t name       : 3;
			uint32_t jarate     : 3;
			uint32_t uber       : 3;
			uint32_t bonk       : 3;
			uint32_t zoom       : 3;
			uint32_t _reserved  : 8; // padding
		};
	} sides;

	union
	{
		uint32_t raw = (1u << 12); // thickness = 1
		struct
		{
			// max is 15 for each one
			// 1111 = 15 [0-15] (1-16)
			uint32_t box_rounding : 4;
			uint32_t healthbar_rounding : 4;
			uint32_t healthbar_margin : 4;
			uint32_t healthbar_thickness : 4;
			uint32_t gap : 4; // health bar gap
			uint32_t text_padding : 4;

			uint32_t _reserved : 8; // padding
		};
	} custom;

	int health	   = 0;
	int team_selection = 0;
};

struct ChamsConfig
{
	bool enabled = false;
	std::vector<std::string> active_materials;
};

struct GlowConfig
{
	union
	{
		uint32_t raw = 0;

		struct
		{
			uint32_t enabled : 1;
			uint32_t stencil : 4;
			uint32_t blur : 4;
			uint32_t weapon : 1;
			uint32_t healthpack : 1;
			uint32_t ammopack : 1;

			uint32_t _reserved : 20; // padding
		};
	} packed;
};

struct AimbotConfig
{
	union
	{
		uint32_t raw = 0;

		struct
		{
			uint32_t enabled : 1;
			uint32_t autoshoot : 1;
			uint32_t viewmodelaim : 1;
			uint32_t draw_fov_indicator : 1;
			uint32_t ignorecloaked : 1;
			uint32_t ignoreubered : 1;
			uint32_t ignorehoovy : 1;
			uint32_t ignorebonked : 1;
			uint32_t waitforcharge : 1;
			uint32_t hold_minigun_spin : 1;

			uint32_t proj_path : 1;

			uint32_t aimmode : 3;
			uint32_t meleemode : 2;
			uint32_t teamselection : 2;
			uint32_t proj_indicator : 2;

			uint32_t _reserved : 12;
		};
	} packed;

	float fov		= 0.0f;
	float max_sim_time	= 0.0f;
	float smoothness	= 0.0f;
	Hotkey *key		= nullptr;
};

struct MiscConfig
{
	union
	{
		uint32_t raw = 0;

		struct
		{
			uint32_t thirdperson : 1;
			uint32_t customfov_enabled : 1;
			uint32_t spectatorlist : 1;
			uint32_t backpack_expander : 1;
			uint32_t sv_pure_bypass : 1;
			uint32_t streamer_mode : 1;
			uint32_t bhop : 1;
			uint32_t autostrafe : 1;
			uint32_t accept_item_drop : 1;
			uint32_t playerlist : 1;
			uint32_t norecoil : 1;
			uint32_t no_viewmodel_bob : 1;

			uint32_t no_engine_sleep : 1;
			uint32_t no_scope_overlay : 1;
			uint32_t no_zoom : 1;
			uint32_t antiafk : 1;
			uint32_t infopanel : 1;
			uint32_t spyalert : 1;

			uint32_t _reserved : 14; // padding
		};
	} packed;

	Hotkey *thirdperson_key	    = nullptr;
	float customfov		    = 90.0f;
	float zoomedfov             = 20.0f;
	float viewmodel_offset[3]   = {0, 0, 0};
	float viewmodel_interp	    = 0.0f;
	float thirdperson_offset[4] = {23.5, 11.5, 8.0f, 1.0f};
	float aspectratio	    = 0;
};

struct TriggerConfig
{
	Hotkey *key	 = nullptr;

	union
	{
		uint32_t raw = 0;

		struct
		{
			uint32_t hitscan : 1;
			uint32_t autobackstab : 2;
			uint32_t autoairblast : 2;
		};
	} packed;
};

struct ColorsConfig
{
	Color red_team	    = {255, 0, 0, 255};
	Color blu_team	    = {0, 255, 255, 255};
	Color aimbot_target = {255, 255, 255, 255};
	Color weapon	    = {255, 255, 255, 255};
	Color menu_accent   = {0, 150, 255, 255};
	Color healthkit	    = {255, 100, 100, 255};
	Color ammopack      = {255, 255, 255, 255};
};

struct RadarConfig
{
	union
	{
		uint32_t raw = 0;

		struct
		{
			uint32_t enabled : 1;
			uint32_t players : 1;
			uint32_t buildings : 1;
			uint32_t objective : 1;
			uint32_t projectiles : 1;

			uint32_t size : 9;
			uint32_t range : 12;
			uint32_t icon_size : 4;

			uint32_t _reserved : 2; // padding
		};
	} packed;
};

struct BacktrackConfig
{
	union
	{
		uint8_t raw = 0;

		struct
		{
			uint8_t enabled : 1;
			uint8_t draw_mode : 2;
		};
	} packed;

	std::vector<std::string> active_materials;
};

struct Configuration
{
	ESPConfig esp;
	ChamsConfig chams;
	GlowConfig glow;
	AimbotConfig aimbot;
	MiscConfig misc;
	TriggerConfig trigger;
	ColorsConfig colors;
	RadarConfig radar;
	WarpConfig warp;
	BacktrackConfig backtrack;
};

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

#define CONFIG_STRING_VECTOR(name, var)                                                                                \
        {name,                                                                                                         \
         SettingType::STRING,                                                                                          \
         [](std::ofstream &f) {                                                                                        \
                 for (size_t i = 0; i < (var).size(); ++i) {                                                           \
                         f << (var)[i] << (i + 1 == (var).size() ? "" : ",");                                          \
                 }                                                                                                     \
         },                                                                                                            \
         [](const std::string &v) {                                                                                    \
                 (var).clear();                                                                                        \
                 std::stringstream ss(v);                                                                              \
                 std::string item;                                                                                     \
                 while (std::getline(ss, item, ',')) {                                                                 \
                         if (!item.empty()) (var).push_back(item);                                                     \
                 }                                                                                                     \
         },                                                                                                            \
         []() {                                                                                                        \
                 std::string res;                                                                                      \
                 for (size_t i = 0; i < (var).size(); ++i) {                                                           \
                         res += (var)[i] + (i + 1 == (var).size() ? "" : ",");                                         \
                 }                                                                                                     \
                 return res;                                                                                           \
         },                                                                                                            \
         [](const std::string &v) {                                                                                    \
                 (var).clear();                                                                                        \
                 std::stringstream ss(v);                                                                              \
                 std::string item;                                                                                     \
                 while (std::getline(ss, item, ',')) {                                                                 \
                         if (!item.empty()) (var).push_back(item);                                                     \
                 }                                                                                                     \
         }}

inline Configuration Config;

namespace Settings
{
	extern bool menu_open;
	inline SettingEntry m_entries[]
	{
		// aimbot
		CONFIG_INT("aimbot raw", Config.aimbot.packed.raw),
		//CONFIG_BOOL("aimbot enabled", Config.aimbot.packed.enabled),
		CONFIG_FLOAT("aimbot fov", Config.aimbot.fov),
		//CONFIG_BOOL("aimbot autoshot", Config.aimbot.packed.autoshoot),
		CONFIG_FLOAT("aimbot max sim time", Config.aimbot.max_sim_time),
		//CONFIG_BOOL("aimbot viewmodel aim", Config.aimbot.packed.viewmodelaim),
		//CONFIG_BOOL("aimbot draw fov indicator", Config.aimbot.packed.draw_fov_indicator),
		//CONFIG_BOOL("aimbot ignore cloaked", Config.aimbot.packed.ignorecloaked),
		//CONFIG_BOOL("aimbot ignore ubered", Config.aimbot.packed.ignoreubered),
		//CONFIG_BOOL("aimbot ignore hoovy", Config.aimbot.packed.ignorehoovy),
		//CONFIG_BOOL("aimbot ignore bonked", Config.aimbot.packed.ignorebonked),
		//CONFIG_INT("aimbot melee", Config.aimbot.packed.meleemode),
		//CONFIG_BOOL("aimbot wait for charge", Config.aimbot.packed.waitforcharge),
		//CONFIG_INT("aimbot mode", Config.aimbot.packed.aimmode),
		CONFIG_FLOAT("aimbot smoothness", Config.aimbot.smoothness),
		//CONFIG_INT("aimbot team mode", Config.aimbot.packed.teamselection),
		//CONFIG_BOOL("aimbot hold minigun spin", Config.aimbot.packed.hold_minigun_spin),
		//CONFIG_BOOL("aimbot indicator", Config.aimbot.packed.proj_indicator),
		CONFIG_KEY("aimbot key", Config.aimbot.key),
		//CONFIG_INT("aimbot path", Config.aimbot.packed.proj_path),

		// esp
		CONFIG_INT("esp raw", Config.esp.packed.raw),
		//CONFIG_BOOL("esp enabled", Config.esp.packed.enabled),
		//CONFIG_BOOL("esp ignore cloaked", Config.esp.packed.ignorecloaked),
		//CONFIG_BOOL("esp buildings", Config.esp.packed.buildings),
		//CONFIG_BOOL("esp name", Config.esp.packed.name),
		//CONFIG_BOOL("esp box", Config.esp.packed.box),
		CONFIG_INT("esp health", Config.esp.health),
		//CONFIG_BOOL("esp weapon", Config.esp.packed.weapon),
		CONFIG_INT("esp conditions", Config.esp.conditions.raw),
		CONFIG_INT("esp team", Config.esp.team_selection),
		CONFIG_INT("esp font", Config.esp.font.selected),
		CONFIG_INT("esp font size", Config.esp.font.size),
		//CONFIG_BOOL("esp class", Config.esp.packed.class_name),
		CONFIG_INT("esp fsides", Config.esp.sides.raw),
		CONFIG_INT("esp custom", Config.esp.custom.raw),

		// glow
		CONFIG_INT("glow raw", Config.glow.packed.raw),

		// misc
		CONFIG_INT("misc raw", Config.misc.packed.raw),
		//CONFIG_BOOL("misc thirdperson", Config.misc.packed.thirdperson),
		CONFIG_KEY("misc thirdperson key", Config.misc.thirdperson_key),
		CONFIG_FLOAT4("misc thirdperson offset", Config.misc.thirdperson_offset),
		//CONFIG_BOOL("misc customfov enabled", Config.misc.packed.customfov_enabled),
		CONFIG_FLOAT("misc customfov", Config.misc.customfov),
		CONFIG_FLOAT("misc customfov zoom", Config.misc.zoomedfov),
		//CONFIG_BOOL("misc spectatorlist", Config.misc.packed.spectatorlist),
		//CONFIG_BOOL("misc backpack_expander", Config.misc.packed.backpack_expander),
		//CONFIG_BOOL("misc sv pure bypass", Config.misc.packed.sv_pure_bypass),
		//CONFIG_BOOL("misc streamer mode", Config.misc.packed.streamer_mode),
		//CONFIG_BOOL("misc bhop", Config.misc.packed.bhop),
		//CONFIG_BOOL("misc autostrafe", Config.misc.packed.autostrafe),
		//CONFIG_BOOL("misc accept item drop", Config.misc.packed.accept_item_drop),
		//CONFIG_BOOL("misc playerlist", Config.misc.packed.playerlist),
		CONFIG_FLOAT3("misc viewmodel offset", Config.misc.viewmodel_offset),
		CONFIG_FLOAT("misc viewmodel interp", Config.misc.viewmodel_interp),
		//CONFIG_BOOL("misc no recoil", Config.misc.packed.norecoil),
		//CONFIG_BOOL("misc no engine sleep", Config.misc.packed.no_engine_sleep),
		//CONFIG_BOOL("misc no scope overlay", Config.misc.packed.no_scope_overlay),
		//CONFIG_BOOL("misc no zoom", Config.misc.packed.no_zoom),
		//CONFIG_BOOL("misc no viewmodel bob", Config.misc.packed.no_viewmodel_bob),
		//CONFIG_BOOL("misc antiafk", Config.misc.packed.antiafk),
		//CONFIG_BOOL("misc info panel", Config.misc.packed.infopanel),
		//CONFIG_BOOL("misc spy alert", Config.misc.packed.spyalert),
		CONFIG_FLOAT("misc aspect ratio", Config.misc.aspectratio),

		//triggerbot
		CONFIG_KEY("trigger key", Config.trigger.key),
		CONFIG_INT("trigger raw", Config.trigger.packed.raw),
		//CONFIG_BOOL("trigger hitscan", Config.trigger.packed.hitscan),
		//CONFIG_INT("trigger autobackstab", Config.trigger.packed.autobackstab),
		//CONFIG_INT("trigger autoairblast", Config.trigger.packed.autoairblast),

		// colors
		CONFIG_FLOAT4("colors red team", Config.colors.red_team),
		CONFIG_FLOAT4("colors blu team", Config.colors.blu_team),
		CONFIG_FLOAT4("colors aimbot target", Config.colors.aimbot_target),
		CONFIG_FLOAT4("colors weapon", Config.colors.weapon),
		CONFIG_FLOAT4("colors accent", Config.colors.menu_accent),

		// warp
		CONFIG_INT("warp raw", Config.warp.packed.raw),
		//CONFIG_BOOL("warp enabled", Config.warp.packed.enabled),
		//CONFIG_INT("warp speed", Config.warp.packed.speed),
		CONFIG_KEY("warp key", Config.warp.key),
		CONFIG_KEY("warp recharge key", Config.warp.recharge_key),

		// radar
		CONFIG_INT("radar raw", Config.radar.packed.raw),
		//CONFIG_BOOL("radar enabled", Config.radar.packed.enabled),
		//CONFIG_INT("radar size", Config.radar.packed.size),
		//CONFIG_INT("radar range", Config.radar.packed.range),
		//CONFIG_BOOL("radar players", Config.radar.packed.players),
		//CONFIG_BOOL("radar buildings", Config.radar.packed.buildings),
		//CONFIG_BOOL("radar objective", Config.radar.packed.objective),
		//CONFIG_BOOL("radar projectiles", Config.radar.packed.projectiles),
		//CONFIG_INT("radar icon size", Config.radar.packed.icon_size),

		// chams
		CONFIG_BOOL("chams enabled", Config.chams.enabled),
		CONFIG_STRING_VECTOR("chams materials", Config.chams.active_materials),

		// backtrack
		CONFIG_INT("backtrack raw", Config.backtrack.packed.raw),
		//CONFIG_BOOL("backtrack enabled", Config.backtrack.packed.enabled),
		//CONFIG_INT("backtrack draw mode", Config.backtrack.packed.draw_mode),
		//CONFIG_STRING_VECTOR("backtrack materials", Config.backtrack.active_materials),
	};

	int Save(const std::string &fullPath);
	int Load(const std::string &fullPath);
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