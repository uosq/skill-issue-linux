#pragma once

#include "../libdetour/libdetour.h"
#include "../settings/settings.h"

#include "../sdk/interfaces/interfaces.h"
#include "../sdk/signatures/signatures.h"

ADD_SIG(CL_CheckForPureServerWhitelist, "engine.so", "83 3D ? ? ? ? 01 7E ? 80 3D ? ? ? ? 00 75")

inline detour_ctx_t CL_CheckForPureServerWhitelist_ctx;
DETOUR_DECL_TYPE(void, originalCheckForPureServerWhitelistFn, void *&pFilesToReload);

inline void Hooked_CL_CheckForPureServerWhitelist(void *&pFilesToReload)
{
	if (Settings::Misc.sv_pure_bypass)
		return;

	DETOUR_ORIG_CALL(&CL_CheckForPureServerWhitelist_ctx, originalCheckForPureServerWhitelistFn, pFilesToReload);
}

inline void HookCheckForPure()
{
	detour_init(&CL_CheckForPureServerWhitelist_ctx, Sigs::CL_CheckForPureServerWhitelist.GetPointer(),
		    (void *)&Hooked_CL_CheckForPureServerWhitelist);
	detour_enable(&CL_CheckForPureServerWhitelist_ctx);

#ifdef DEBUG
	constexpr Color_t color{100, 255, 100, 255};
	interfaces::Cvar->ConsoleColorPrintf(color, "CL_CheckForPureServerWhitelist hooked\n");
#endif
}