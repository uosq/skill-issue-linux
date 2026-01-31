#pragma once

#include "../sdk/interfaces/interfaces.h"
#include "../settings.h"
#include "../libdetour/libdetour.h"
#include <cstdlib>

#include "../features/lua/hooks.h"
#include "../features/lua/api.h"
#include "../features/lua/classes.h"

using Host_ShutdownFn = bool(*)(void);
static Host_ShutdownFn originalHost_ShutdownFn = nullptr;

DETOUR_DECL_TYPE(void, originalHost_ShutdownFn, void);

static detour_ctx_t shutdownctx;

static void HookedHost_ShutdownFn(void)
{
	if (LuaHookManager::HasHooks("GameShutdown"))
		LuaHookManager::Call(Lua::m_luaState, "GameShutdown");

	DETOUR_ORIG_CALL(&shutdownctx, originalHost_ShutdownFn);
}

static void HookHost_Shutdown()
{
	originalHost_ShutdownFn = reinterpret_cast<Host_ShutdownFn>(sigscan_module("engine.so", "80 3D ? ? ? ? 00 0F 85 ? ? ? ? 55 31 F6"));
	detour_init(&shutdownctx, (void*)originalHost_ShutdownFn, (void*)&HookedHost_ShutdownFn);
	detour_enable(&shutdownctx);

	interfaces::Cvar->ConsolePrintf("Host_Shutdown hooked\n");
}