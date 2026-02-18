#pragma once

#include "../imgui/imgui.h"
#include "../imgui/imgui_stdlib.h"
#include "../settings/settings.h"
#include "../sdk/helpers/helper.h"
#include "../features/antiaim/antiaim.h"
#include "../imgui/TextEditor.h"
#include "../features/lua/api.h"
#include "console.h"
#include "../features/entitylist/entitylist.h"
#include "../sdk/definitions/eteam.h"
#include "../features/aimbot/melee/aimbot_melee.h"
#include "../features/aimbot/utils/utils.h"
#include "../features/triggerbot/triggerbot.h"

enum TabMenu
{
	TAB_AIMBOT = 0,
	TAB_ESP,
	TAB_MISC,
	TAB_TRIGGER,
	TAB_ANTIAIM,
	TAB_LUA,
	TAB_NETVARS,
	TAB_RADAR,
	TAB_CONFIG
};

namespace GUI
{
	extern int tab;
	extern TextEditor editor;

	extern int selectedIndex;
	extern std::vector<std::string> configs;
	extern std::string newConfigName;
	extern bool firstOpenConfigTab;
	extern bool openDeletePopup;

	void RunMainWindow();
	void RunPlayerList();
	void RunSpectatorList();
	void Init();
}