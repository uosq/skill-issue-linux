#pragma once

#include "../imgui/imgui.h"
#include "../imgui/imgui_stdlib.h"
#include "../sdk/definitions/eteam.h"
#include "../sdk/helpers/helper.h"
#include "../settings/settings.h"

#include "../features/aimbot/melee/aimbot_melee.h"
#include "../features/aimbot/utils/utils.h"
#include "../features/entitylist/entitylist.h"
#include "../features/triggerbot/triggerbot.h"

namespace GUI
{
	void RunMainWindow();
	void RunPlayerList();
	void RunSpectatorList();
	void Init();
} // namespace GUI