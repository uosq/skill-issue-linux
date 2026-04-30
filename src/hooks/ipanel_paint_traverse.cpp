#include "ipanel_paint_traverse.h"

#include "../sdk/definitions/ipanel.h"
#include "../sdk/interfaces/interfaces.h"
#include "../sdk/definitions/fnv.h"

#include "../settings/settings.h"

#include "../hooks.h"

using PaintTraverseFn = void (*)(IPanel* rdi, VPANEL vguiPanel, bool forceRepaint, bool allowForce);

static void PaintTraverse(IPanel* rdi, VPANEL vguiPanel, bool forceRepaint, bool allowForce)
{
	const char *panelName = interfaces::VGui->GetName(vguiPanel);

	// https://github.com/rei-2/Amalgam/blob/master/Amalgam/src/Hooks/IPanel_PaintTraverse.cpp
	if (Settings::Misc.streamer_mode)
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

	if (Settings::Misc.no_scope_overlay)
	{
		if (fnv::Hash(panelName) == fnv::HashConst("HudScope"))
			return;
	}

	auto original = VMTHooks::VGui.GetOriginal<PaintTraverseFn>(42);
	original(rdi, vguiPanel, forceRepaint, allowForce);
}

void HookPaintTraverse()
{
	VMTHooks::VGui.Hook(42, &PaintTraverse);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("IPanel::PaintTraverse hooked\n", color);
#endif
}