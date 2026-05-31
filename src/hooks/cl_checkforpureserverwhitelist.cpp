#include "../core/core.h"

#include "../sdk/signatures/signatures.h"

#include "../settings/settings.h"
#include "../thirdparty/libdetour/libdetour.h"

#include "../features/hook_initializer/initializer.h"

ADD_SIG(CL_CheckForPureServerWhitelist, "engine.so", "83 3D ? ? ? ? 01 7E ? 80 3D ? ? ? ? 00 75")

DETOUR_DECL_TYPE(void, originalCheckForPureServerWhitelistFn, void *&pFilesToReload);
detour_ctx_t CL_CheckForPureServerWhitelist_ctx;

void Hooked_CL_CheckForPureServerWhitelist(void *&pFilesToReload)
{
	if (gApp->IsInitialized())
	{
		if (Config.misc.packed.sv_pure_bypass)
			return;
	}

	DETOUR_ORIG_CALL(&CL_CheckForPureServerWhitelist_ctx, originalCheckForPureServerWhitelistFn, pFilesToReload);
}

void HookCheckForPure()
{
	detour_init(&CL_CheckForPureServerWhitelist_ctx, Sigs::CL_CheckForPureServerWhitelist.GetPointer(),
		    (void *)&Hooked_CL_CheckForPureServerWhitelist);
	detour_enable(&CL_CheckForPureServerWhitelist_ctx);

#ifdef DEBUG
	constexpr Color_t color{100, 255, 100, 255};
	interfaces::Cvar->ConsoleColorPrintf(color, "CL_CheckForPureServerWhitelist hooked\n");
#endif
}

MARK_FOR_INIT(HookCheckForPure)