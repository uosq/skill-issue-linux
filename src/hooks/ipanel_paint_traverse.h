#pragma once

#include "../tracy/tracy/Tracy.hpp"

#include "../sdk/definitions/ipanel.h"
#include "../sdk/interfaces/interfaces.h"
#include "../sdk/helpers/helper.h"
#include "../settings.h"

DECLARE_VTABLE_HOOK(PaintTraverse, void, (IPanel* thisptr, VPANEL vguiPanel, bool forceRepaint, bool allowForce))
{
	ZoneScoped;

	const char* panelName = interfaces::VGui->GetName(vguiPanel);

	// https://github.com/rei-2/Amalgam/blob/master/Amalgam/src/Hooks/IPanel_PaintTraverse.cpp
	if (g_Settings.misc.streamer_mode)
	{
		switch (fnv::Hash(panelName))
		{
			case fnv::HashConst("SteamFriendsList"):
			case fnv::HashConst("avatar"):
			case fnv::HashConst("RankPanel"):
			case fnv::HashConst("ModelContainer"):
			case fnv::HashConst("ServerLabelNew"):
			return;
		}
	}

	originalPaintTraverse(thisptr, vguiPanel, forceRepaint, allowForce);
}

inline void HookPaintTraverse()
{
	INSTALL_VTABLE_HOOK(PaintTraverse, interfaces::VGui, 42);

	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("IPanel::PaintTraverse hooked\n", color);
}