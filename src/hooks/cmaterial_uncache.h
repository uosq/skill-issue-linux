#pragma once

#include "../sdk/MaterialManager/materialmanager.h"
#include "../libdetour/libdetour.h"
#include <cstdlib>

inline detour_ctx_t uncache_ctx;
DETOUR_DECL_TYPE(void, original_Uncache, IMaterial* mat, bool bPreserveVars);

inline void HookedUncache(IMaterial* mat, bool bPreserveVars)
{
	if (MaterialManager::MaterialExists(mat->GetName()))
		return;;

	DETOUR_ORIG_CALL(&uncache_ctx, original_Uncache, mat, bPreserveVars);
}

inline void HookCMaterial_Uncache(void)
{
	void* func = sigscan_module("materialsystem.so", "55 48 89 E5 41 56 41 55 41 54 49 89 FC 53 89 F3 48 81 EC 10 01 00 00");
	detour_init(&uncache_ctx, func, (void*)&HookedUncache);
	detour_enable(&uncache_ctx);

	#ifdef DEBUG
	Color_t color = {255, 255, 255, 255};
	interfaces::Cvar->ConsoleColorPrintf(color, "CMaterial::Uncache hooked\n");
	#endif
}