#pragma once

#include "../vtables.h"
#include "../sdk/helpers/helper.h"
#include "../features/entitylist/entitylist.h"
#include "../features/lua/hookmgr.h"
#include "../features/lua/api.h"
#include "../features/lua/classes.h"
#include "../features/warp/warp.h"

DECLARE_VTABLE_HOOK(LevelShutdown, void, (CHLClient* thisptr))
{
	EntityList::Clear();
	Warp::Reset();

	if (LuaHookManager::HasHooks("LevelShutdown"))
		LuaHookManager::Call(Lua::m_luaState, "LevelShutdown");

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