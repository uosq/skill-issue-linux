#include "host_shutdown.h"

#include "../libdetour/libdetour.h"

#include "../features/scriptmanager/scriptmanager.h"
#include "../features/chams/chams.h"
#include "../features/materialregistry/reg.h"
#include "../features/playerlist/playerlist.h"

DETOUR_DECL_TYPE(void, originalHost_ShutdownFn, void);
detour_ctx_t shutdownctx;

void HookedHost_ShutdownFn(void)
{
	features::chams.OnGameShutdown();
	features::playerlist.Shutdown();
	features::material_registry.Shutdown();

	features::scriptmanager.CallHooks("GameShutdown");
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