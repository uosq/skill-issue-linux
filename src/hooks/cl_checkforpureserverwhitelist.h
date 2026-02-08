#pragma once

#include "../sdk/interfaces/interfaces.h"
#include "../settings.h"
#include "../libdetour/libdetour.h"

inline detour_ctx_t CL_CheckForPureServerWhitelist_ctx;
DETOUR_DECL_TYPE(void, originalCheckForPureServerWhitelistFn, void*& pFilesToReload);


inline void Hooked_CL_CheckForPureServerWhitelist(void*& pFilesToReload)
{
	if (Settings::misc.sv_pure_bypass)
		return;

	DETOUR_ORIG_CALL(&CL_CheckForPureServerWhitelist_ctx, originalCheckForPureServerWhitelistFn, pFilesToReload);
}

inline void HookCheckForPure()
{
	void* original = sigscan_module("engine.so", "83 3D ? ? ? ? 01 7E ? 80 3D ? ? ? ? 00 75");
	detour_init(&CL_CheckForPureServerWhitelist_ctx, original, (void*)&Hooked_CL_CheckForPureServerWhitelist);
	detour_enable(&CL_CheckForPureServerWhitelist_ctx);

	constexpr Color_t color{100, 255, 100, 255};
	interfaces::Cvar->ConsoleColorPrintf(color, "CL_CheckForPureServerWhitelist hooked\n");
}