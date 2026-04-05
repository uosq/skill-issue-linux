#include "cl_checkforpureserverwhitelist.h"

DETOUR_DECL_TYPE(void, originalCheckForPureServerWhitelistFn, void *&pFilesToReload);
detour_ctx_t CL_CheckForPureServerWhitelist_ctx;

void Hooked_CL_CheckForPureServerWhitelist(void *&pFilesToReload)
{
	if (Settings::Misc.sv_pure_bypass)
		return;

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