#include "settings.h"
#include <fstream>

bool Settings::menu_open = false;

namespace Settings
{
	namespace AntiAim
	{
		bool enabled = false;
		int pitch_mode = 0;
		int real_yaw_mode = 0;
		int fake_yaw_mode = 0;
		float spin_speed = 1.0f;

		bool warp_enabled = false;
		std::string warp_key = "";
		std::string warp_recharge_key = "";
		std::string warp_dt_key = "";
		int warp_speed = 1;
	}
}

namespace Settings
{
	namespace ESP
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
	}
}

namespace Settings
{
	namespace Aimbot
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
		int mode = 0;
		int melee = 0;
		int teamMode = 0;
		float smoothness = 0.0f;
	}
}

namespace Settings
{
	namespace Misc
	{
		bool thirdperson = false;
		std::string thirdperson_key = "";
		bool customfov_enabled = false;
		float customfov = 0.0f;
		bool spectatorlist = false;
		bool backpack_expander = false;
		bool sv_pure_bypass = false;
		bool streamer_mode = false;
		bool bhop = false;
		bool accept_item_drop = false;
		bool playerlist = false;
		bool norecoil = false;
		float viewmodel_offset[3] = {0.0f, 0.0f, 0.0f};
		float viewmodel_interp = 0.0f;
	}
}

namespace Settings
{
	namespace Triggerbot
	{
		bool enabled = false;
		bool hitscan = false;
		int autobackstab = 0;
		std::string key = "";
		int autoairblast = 0;
	}
}

namespace Settings
{
	namespace Colors
	{
		Color red_team = {255, 0, 0, 255};
		Color blu_team = {0, 255, 255, 255};
		Color aimbot_target = {255, 255, 255, 255};
		Color weapon = {255, 255, 255, 255};
	}
}

namespace Settings
{
	namespace Radar
	{
		int size = 120;
		int range = 3000;
		int icon_size = 3;
		bool enabled = false;
		bool players = false;
		bool buildings = false;
		bool objective = false;
		bool projectiles = false;
	}
}

#define REG_SETTING(name, path, type, settingType) \
    Settings::GetOptionMap()[#name] = \
    std::make_unique<SettingEntry<type>>(path, settingType, #name);

// probably not very efficient code, but it works so fuck it ig
void Settings::RegisterOptions()
{
	// aimbot
	REG_SETTING(aimbot enabled, Aimbot::enabled, bool, SettingType::BOOL)
	REG_SETTING(aimbot fov, Aimbot::fov, float, SettingType::FLOAT)
	REG_SETTING(aimbot key, Aimbot::key, std::string, SettingType::STRING)
	REG_SETTING(aimbot autoshot, Aimbot::autoshoot, bool, SettingType::BOOL)
	REG_SETTING(aimbot max sim time, Aimbot::max_sim_time, float, SettingType::FLOAT)
	REG_SETTING(aimbot viewmodel aim, Aimbot::viewmodelaim, bool, SettingType::BOOL)
	REG_SETTING(aimbot draw fov indicator, Aimbot::draw_fov_indicator, bool, SettingType::BOOL)
	REG_SETTING(aimbot ignore cloaked, Aimbot::ignorecloaked, bool, SettingType::BOOL)
	REG_SETTING(aimbot ignore ubered, Aimbot::ignoreubered, bool, SettingType::BOOL)
	REG_SETTING(aimbot ignore hoovy, Aimbot::ignorehoovy, bool, SettingType::BOOL)
	REG_SETTING(aimbot ignore bonked, Aimbot::ignorebonked, bool, SettingType::BOOL)
	REG_SETTING(aimbot melee, Aimbot::melee, int, SettingType::INT)
	REG_SETTING(aimbot wait for charge, Aimbot::waitforcharge, bool, SettingType::BOOL)
	REG_SETTING(aimbot mode, Aimbot::mode, int, SettingType::INT)
	REG_SETTING(aimbot smoothness, Aimbot::smoothness, float, SettingType::FLOAT)
	REG_SETTING(aimbot team mode, Aimbot::teamMode, int, SettingType::INT)
	REG_SETTING(aimbot hold minigun spin, Aimbot::hold_minigun_spin, bool, SettingType::BOOL)

	// esp
	REG_SETTING(esp enabled, ESP::enabled, bool, SettingType::BOOL)
	REG_SETTING(esp ignore cloaked, ESP::ignorecloaked, bool, SettingType::BOOL)
	REG_SETTING(esp buildings, ESP::buildings, bool, SettingType::BOOL)
	REG_SETTING(esp name, ESP::name, bool, SettingType::BOOL)
	REG_SETTING(esp box, ESP::box, bool, SettingType::BOOL)
	REG_SETTING(esp health, ESP::healthbar, bool, SettingType::BOOL)
	REG_SETTING(esp chams, ESP::chams, bool, SettingType::BOOL)
	REG_SETTING(esp stencil, ESP::stencil, int, SettingType::INT)
	REG_SETTING(esp blur, ESP::blur, int, SettingType::INT)
	REG_SETTING(esp weapon, ESP::weapon, bool, SettingType::BOOL)
	REG_SETTING(esp health, ESP::healthbar, bool, SettingType::BOOL)

	// misc
	REG_SETTING(misc thirdperson, Misc::thirdperson, bool, SettingType::BOOL)
	REG_SETTING(misc thirdperson key, Misc::thirdperson_key, std::string, SettingType::STRING)
	REG_SETTING(misc customfov enabled, Misc::customfov_enabled, bool, SettingType::BOOL)
	REG_SETTING(misc customfov, Misc::customfov, float, SettingType::FLOAT)
	REG_SETTING(misc spectatorlist, Misc::spectatorlist, bool, SettingType::BOOL)
	REG_SETTING(misc backpack_expander, Misc::backpack_expander, bool, SettingType::BOOL)
	REG_SETTING(misc sv pure bypass, Misc::sv_pure_bypass, bool, SettingType::BOOL)
	REG_SETTING(misc streamer mode, Misc::streamer_mode, bool, SettingType::BOOL)
	REG_SETTING(misc bhop, Misc::bhop, bool, SettingType::BOOL)
	REG_SETTING(misc accept item drop, Misc::accept_item_drop, bool, SettingType::BOOL)
	REG_SETTING(misc playerlist, Misc::playerlist, bool, SettingType::BOOL)
	REG_SETTING(misc viewmodel offset x, Misc::viewmodel_offset[0], float, SettingType::FLOAT)
	REG_SETTING(misc viewmodel offset y, Misc::viewmodel_offset[1], float, SettingType::FLOAT)
	REG_SETTING(misc viewmodel offset z, Misc::viewmodel_offset[2], float, SettingType::FLOAT)
	REG_SETTING(misc viewmodel interp, Misc::viewmodel_interp, float, SettingType::FLOAT)
	REG_SETTING(misc no recoil, Misc::norecoil, bool, SettingType::BOOL)

	//triggerbot
	REG_SETTING(trigger enabled, Triggerbot::enabled, bool, SettingType::BOOL)
	REG_SETTING(trigger hitscan, Triggerbot::hitscan, bool, SettingType::BOOL)
	REG_SETTING(trigger autobackstab, Triggerbot::autobackstab, int, SettingType::INT)
	REG_SETTING(trigger key, Triggerbot::key, std::string, SettingType::STRING)
	REG_SETTING(trigger autoairblast, Triggerbot::autoairblast, int, SettingType::INT)

	// colors
	REG_SETTING(colors red team r, Colors::red_team[0], uint8_t, SettingType::UINT8)
	REG_SETTING(colors red team g, Colors::red_team[1], uint8_t, SettingType::UINT8)
	REG_SETTING(colors red team b, Colors::red_team[2], uint8_t, SettingType::UINT8)
	REG_SETTING(colors red team a, Colors::red_team[3], uint8_t, SettingType::UINT8)
	REG_SETTING(colors blu team r, Colors::blu_team[0], uint8_t, SettingType::UINT8)
	REG_SETTING(colors blu team g, Colors::blu_team[1], uint8_t, SettingType::UINT8)
	REG_SETTING(colors blu team b, Colors::blu_team[2], uint8_t, SettingType::UINT8)
	REG_SETTING(colors blu team a, Colors::blu_team[3], uint8_t, SettingType::UINT8)
	REG_SETTING(colors aimbot target r, Colors::aimbot_target[0], uint8_t, SettingType::UINT8)
	REG_SETTING(colors aimbot target g, Colors::aimbot_target[1], uint8_t, SettingType::UINT8)
	REG_SETTING(colors aimbot target b, Colors::aimbot_target[2], uint8_t, SettingType::UINT8)
	REG_SETTING(colors aimbot target a, Colors::aimbot_target[3], uint8_t, SettingType::UINT8)
	REG_SETTING(colors weapon r, Colors::weapon[0], uint8_t, SettingType::UINT8)
	REG_SETTING(colors weapon g, Colors::weapon[1], uint8_t, SettingType::UINT8)
	REG_SETTING(colors weapon b, Colors::weapon[2], uint8_t, SettingType::UINT8)
	REG_SETTING(colors weapon a, Colors::weapon[3], uint8_t, SettingType::UINT8)

	// antiaim
	REG_SETTING(antiaim enabled, AntiAim::enabled, bool, SettingType::BOOL)
	REG_SETTING(antiaim pitch mode, AntiAim::pitch_mode, int, SettingType::INT)
	REG_SETTING(antiaim real yaw mode, AntiAim::real_yaw_mode, int, SettingType::INT)
	REG_SETTING(antiaim fake yaw mode, AntiAim::fake_yaw_mode, int, SettingType::INT)
	REG_SETTING(antiaim spin speed, AntiAim::spin_speed, float, SettingType::FLOAT)

	// warp
	REG_SETTING(warp enabled, AntiAim::warp_enabled, bool, SettingType::BOOL)
	REG_SETTING(warp speed, AntiAim::warp_speed, int, SettingType::INT)
	REG_SETTING(warp key, AntiAim::warp_key, std::string, SettingType::STRING)
	REG_SETTING(warp recharge key, AntiAim::warp_recharge_key, std::string, SettingType::STRING)
	REG_SETTING(doubletap key, AntiAim::warp_dt_key, std::string, SettingType::STRING)

	// radar
	REG_SETTING(radar enabled, Radar::enabled, bool, SettingType::BOOL)
	REG_SETTING(radar size, Radar::size, int, SettingType::INT)
	REG_SETTING(radar range, Radar::range, int, SettingType::INT)
	REG_SETTING(radar players, Radar::players, bool, SettingType::BOOL)
	REG_SETTING(radar buildings, Radar::buildings, bool, SettingType::BOOL)
	REG_SETTING(radar objective, Radar::objective, bool, SettingType::BOOL)
	REG_SETTING(radar projectiles, Radar::projectiles, bool, SettingType::BOOL);
	REG_SETTING(radar icon size, Radar::icon_size, int, SettingType::INT)
}

void Settings::SaveAll(const std::string &fullPath)
{
	// clear the file
	std::ofstream(fullPath, std::ios::trunc).close();

	// in case my goofy ass forget about this
	// doing [_, setting] is essentially the same thing
	// as `for _, setting in pairs(GetOptionMap())` in lua
	for (auto& [_, setting] : GetOptionMap())
        	setting->Save(fullPath);
}

void Settings::LoadAll(const std::string &fullPath)
{
	// in case my goofy ass forget about this
	// doing [_, setting] is essentially the same thing
	// as `for _, setting in pairs(GetOptionMap())` in lua
	for (auto& [_, setting] : GetOptionMap())
        	setting->Load(fullPath);
}