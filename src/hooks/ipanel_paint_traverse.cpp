#include "../sdk/definitions/ipanel.h"
#include "../sdk/interfaces/interfaces.h"
#include "../sdk/definitions/fnv.h"

#include "../features/misc/misc.h"
#include "../features/hookmanager/hookmanager.hpp"

using PaintTraverseFn = void (*)(IPanel* rdi, VPANEL vguiPanel, bool forceRepaint, bool allowForce);

INIT_VTABLE_HOOK(PaintTraverse, void, (IPanel* rdi, VPANEL vguiPanel, bool forceRepaint, bool allowForce), interfaces::VGui, 42)
{
	const char *panelName = interfaces::VGui->GetName(vguiPanel);

	// https://github.com/rei-2/Amalgam/blob/master/Amalgam/src/Hooks/IPanel_PaintTraverse.cpp
	if (config::streamer_mode::enabled.Get())
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

	if (config::no_scope_overlay::enabled.Get())
	{
		if (fnv::Hash(panelName) == fnv::HashConst("HudScope"))
			return;
	}

	original_PaintTraverse(rdi, vguiPanel, forceRepaint, allowForce);
}