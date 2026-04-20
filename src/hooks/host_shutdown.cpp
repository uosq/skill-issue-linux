#include "host_shutdown.h"

#include "../libdetour/libdetour.h"

#include "../features/angelscript/api/libraries/hooks/hooks.h"
#include "../features/chams/chams.h"

DETOUR_DECL_TYPE(void, originalHost_ShutdownFn, void);
detour_ctx_t shutdownctx;

void HookedHost_ShutdownFn(void)
{
	Chams::OnGameShutdown();

	Hooks_CallHooks("GameShutdown");
	DETOUR_ORIG_CALL(&shutdownctx, originalHost_ShutdownFn);
}

void HookHost_Shutdown()
{
	detour_init(&shutdownctx, Sigs::Host_Shutdown.GetPointer(), (void *)&HookedHost_ShutdownFn);
	detour_enable(&shutdownctx);

#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("Host_Shutdown hooked\n");
#endif
}