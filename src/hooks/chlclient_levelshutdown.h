#pragma once

#include "../vtables.h"

#include "../sdk/helpers/helper.h"

#include "../features/aimbot/projectile/projectile.h"
#include "../features/backtrack/backtrack.h"
#include "../features/bhop/bhop.h"
#include "../features/entitylist/entitylist.h"
#include "../features/spectators/spectators.h"
#include "../features/warp/warp.h"

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

DECLARE_VTABLE_HOOK(LevelShutdown, void, (CHLClient * thisptr))
{
	EntityList::Clear();
	Warp::Reset();
	Backtrack::Reset();
	gAimProjectile.Reset();
	gAimProjectile.ResetIndicator();
	Bhop::Reset();
	Spectators::OnLevelShutdown();

	Hooks_CallHooks("LevelShutdown");
	originalLevelShutdown(thisptr);
}

static void HookLevelShutdown()
{
	INSTALL_VTABLE_HOOK(LevelShutdown, interfaces::ClientDLL, 7);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("BaseClientDll::LevelShutdown hooked\n", color);
#endif
}