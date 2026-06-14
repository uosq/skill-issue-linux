#include "../core/core.h"

#include "../sdk/signatures/signatures.h"
#include "../thirdparty/libdetour/libdetour.h"

#include "../features/gui/gui.h"

#include "../features/MaterialManager/materialmanager.h"
#include "../features/scriptmanager/scriptmanager.h"
#include "../features/chams/chams.h"
#include "../features/playerlist/playerlist.h"
#include "../features/hook_initializer/initializer.h"
#include "../features/network/network.h"

ADD_SIG(Host_Shutdown, "engine.so", "80 3D ? ? ? ? 00 0F 85 ? ? ? ? 55 31 F6")

DETOUR_DECL_TYPE(void, originalHost_ShutdownFn, void);
detour_ctx_t shutdownctx;

void HookedHost_ShutdownFn(void)
{
	if (gApp->IsInitialized())
	{
		features::chams.OnGameShutdown();
		features::playerlist.Shutdown();
		features::network.shutdown();
		features::gui.shutdown();
		features::materials.Uninitialize();
	
		features::scriptmanager.CallHooks("GameShutdown");
	}

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

MARK_FOR_INIT(HookHost_Shutdown)