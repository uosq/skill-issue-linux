#pragma once

#include <cstdlib>

#include "../libdetour/libdetour.h"
#include "../settings/settings.h"

#include "../sdk/interfaces/interfaces.h"
#include "../sdk/signatures/signatures.h"

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

ADD_SIG(Host_Shutdown, "engine.so", "80 3D ? ? ? ? 00 0F 85 ? ? ? ? 55 31 F6")

inline detour_ctx_t shutdownctx;
DETOUR_DECL_TYPE(void, originalHost_ShutdownFn, void);

static void HookedHost_ShutdownFn(void)
{
	Hooks_CallHooks("GameShutdown");
	DETOUR_ORIG_CALL(&shutdownctx, originalHost_ShutdownFn);
}

static void HookHost_Shutdown()
{
	detour_init(&shutdownctx, Sigs::Host_Shutdown.GetPointer(), (void *)&HookedHost_ShutdownFn);
	detour_enable(&shutdownctx);

#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("Host_Shutdown hooked\n");
#endif
}