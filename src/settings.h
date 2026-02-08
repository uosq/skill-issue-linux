#pragma once

#include "sdk/definitions/color.h"
#include <cstdint>
#include <string>
#include <unistd.h>
#include <unordered_map>

enum class SettingType {
    BOOL,
    INT,
    FLOAT,
    STRING
};

struct SettingEntry {
    void* ptr;
    SettingType type;
};

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

enum class AutoAirblastMode
{
	NONE = 0,
	LEGIT, RAGE
};

struct Settings_Antiaim
{
	bool enabled = false;
	PitchMode pitch_mode = PitchMode::NONE;
	YawMode real_yaw_mode = YawMode::NONE;
	YawMode fake_yaw_mode = YawMode::NONE;
	float spin_speed = 0;
};

struct Settings_ESP
{
	bool enabled = false;
	bool ignorecloaked = false;
	bool buildings = false;
	bool name = false;
	bool box = false;
	bool healthbar = false;
	bool chams = false;
	int stencil = 0;
	int blur = 0;
	bool weapon = false;
};

struct Settings_Aimbot
{
	bool enabled = false;
	float fov = 0.0f;
	std::string key = "";
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
	AimbotMode mode = AimbotMode::PLAIN;
	MeleeMode melee = MeleeMode::NONE;
	TeamMode teamMode = TeamMode::ONLYENEMY;
	float smoothness = 10.0f;
};

struct Settings_Misc
{
	bool thirdperson = false;
	std::string thirdperson_key = "";
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
	bool norecoil_hide = false;

	float viewmodel_offset[3] = {0.0, 0.0, 0.0};
	float viewmodel_interp = 0.0f;
};

struct Settings_Triggerbot
{
	bool enabled = false;
	bool hitscan = false;
	AutoBackstabMode autobackstab = AutoBackstabMode::NONE;
	std::string key = "";
	AutoAirblastMode autoairblast = AutoAirblastMode::NONE;
};

struct Settings_Colors
{
	Color red_team = {255, 0, 0, 255};
	Color blu_team = {0, 255, 255, 255};
	Color aimbot_target = {255, 255, 255, 255};
	Color weapon = {255, 255, 255, 255};
};

struct Settings_Radar
{
	int size = 120;
	int range = 3000;
	int icon_size = 3;
	bool enabled = false;
	bool players = false;
	bool buildings = false;
	bool objective = false;
	bool projectiles = false;
};

namespace Settings
{
	extern Settings_Misc misc;
	extern Settings_Aimbot aimbot;
	extern Settings_ESP esp;
	extern Settings_Triggerbot triggerbot;
	extern Settings_Antiaim antiaim;
	extern Settings_Colors colors;
	extern Settings_Radar radar;

	extern bool menu_open;
	extern std::unordered_map<std::string, SettingEntry> m_optionMap;

	void RegisterOptions(void);
};