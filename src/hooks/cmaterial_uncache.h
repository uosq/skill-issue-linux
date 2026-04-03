#pragma once

#include "../libdetour/libdetour.h"
#include "../sdk/MaterialManager/materialmanager.h"
#include <cstdlib>

#include "../sdk/signatures/signatures.h"

ADD_SIG(CMaterial_Uncache, "materialsystem.so", "55 48 89 E5 41 56 41 55 41 54 49 89 FC 53 89 F3 48 81 EC 10 01 00 00")

inline detour_ctx_t uncache_ctx;
DETOUR_DECL_TYPE(void, original_Uncache, IMaterial *mat, bool bPreserveVars);

inline void HookedUncache(IMaterial *mat, bool bPreserveVars)
{
	// hopefully fix the crash that happens here
	if (mat == nullptr)
	{
		DETOUR_ORIG_CALL(&uncache_ctx, original_Uncache, mat, bPreserveVars);
		return;
	}

	if (MaterialManager::MaterialExists(mat->GetName()))
		return;

	DETOUR_ORIG_CALL(&uncache_ctx, original_Uncache, mat, bPreserveVars);
}

inline void HookCMaterial_Uncache(void)
{
	detour_init(&uncache_ctx, Sigs::CMaterial_Uncache.GetPointer(), (void *)&HookedUncache);
	detour_enable(&uncache_ctx);

#ifdef DEBUG
	Color_t color = {255, 255, 255, 255};
	interfaces::Cvar->ConsoleColorPrintf(color, "CMaterial::Uncache hooked\n");
#endif
}