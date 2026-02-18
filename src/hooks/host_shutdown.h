#pragma once

#include "../sdk/interfaces/interfaces.h"
#include "../settings/settings.h"
#include "../libdetour/libdetour.h"
#include <cstdlib>

#include "../features/lua/hookmgr.h"
#include "../features/lua/api.h"
#include "../features/lua/classes.h"

inline detour_ctx_t shutdownctx;
DETOUR_DECL_TYPE(void, originalHost_ShutdownFn, void);

static void HookedHost_ShutdownFn(void)
{
	if (LuaHookManager::HasHooks("GameShutdown"))
		LuaHookManager::Call(Lua::m_luaState, "GameShutdown");

	DETOUR_ORIG_CALL(&shutdownctx, originalHost_ShutdownFn);
}

static void HookHost_Shutdown()
{
	void* original = sigscan_module("engine.so", "80 3D ? ? ? ? 00 0F 85 ? ? ? ? 55 31 F6");
	detour_init(&shutdownctx, original, (void*)&HookedHost_ShutdownFn);
	detour_enable(&shutdownctx);

	#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("Host_Shutdown hooked\n");
	#endif
}