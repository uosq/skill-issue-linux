#include "cmaterial_uncache.h"

#include "../libdetour/libdetour.h"
#include "../sdk/MaterialManager/materialmanager.h"

DETOUR_DECL_TYPE(void, original_Uncache, IMaterial *mat, bool bPreserveVars);
detour_ctx_t uncache_ctx;

void HookedUncache(IMaterial *mat, bool bPreserveVars)
{
	// hopefully fix the crash that happens here
	if (mat == nullptr)
	{
		DETOUR_ORIG_CALL(&uncache_ctx, original_Uncache, mat, bPreserveVars);
		return;
	}

	const char* name = mat->GetName();

	if (name == nullptr) return;

	if (MaterialManager::MaterialExists(name))
		return;

	DETOUR_ORIG_CALL(&uncache_ctx, original_Uncache, mat, bPreserveVars);
}

void HookCMaterial_Uncache(void)
{
	detour_init(&uncache_ctx, Sigs::CMaterial_Uncache.GetPointer(), (void *)&HookedUncache);
	detour_enable(&uncache_ctx);

#ifdef DEBUG
	Color_t color = {255, 255, 255, 255};
	interfaces::Cvar->ConsoleColorPrintf(color, "CMaterial::Uncache hooked\n");
#endif
}