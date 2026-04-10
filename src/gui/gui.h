#pragma once

#include "../imgui/imgui.h"
#include "../imgui/imgui_stdlib.h"
#include "../sdk/definitions/eteam.h"
#include "../sdk/helpers/helper.h"
#include "../settings/settings.h"

#include "../features/aimbot/melee/aimbot_melee.h"
#include "../features/aimbot/utils/utils.h"
#include "../features/antiaim/antiaim.h"
#include "../features/entitylist/entitylist.h"
#include "../features/triggerbot/triggerbot.h"

enum TabMenu {
	TAB_AIMBOT = 0,
	TAB_ESP,
	TAB_MISC,
	TAB_TRIGGER,
	TAB_ANTIAIM,
	TAB_LUA,
	TAB_NETVARS,
	TAB_RADAR,
	TAB_CONFIG,
	TAB_LOGS,
};

namespace GUI {
	extern int tab;

	void RunMainWindow();
	void RunPlayerList();
	void RunSpectatorList();
	void Init();
} // namespace GUI