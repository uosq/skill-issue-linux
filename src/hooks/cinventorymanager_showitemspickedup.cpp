#include "cinventorymanager_showitemspickedup.h"

DETOUR_DECL_TYPE(bool, original_ShowItemsPickedUpFn, void *thisptr, bool bForce, bool bReturnToGame, bool bNoPanel);
detour_ctx_t showitemsctx;

bool HookedShowItemsPickedUpFn(void *thisptr, bool bForce, bool bReturnToGame, bool bNoPanel)
{
	if (Settings::Misc.accept_item_drop)
	{
		interfaces::Cvar->ConsolePrintf("Collected item drop\n");
		DETOUR_ORIG_CALL(&showitemsctx, original_ShowItemsPickedUpFn, thisptr, true, true, true);
		return false;
	}

	bool ret;
	DETOUR_ORIG_GET(&showitemsctx, ret, original_ShowItemsPickedUpFn, thisptr, bForce, bReturnToGame, bNoPanel);
	return ret;
}

void HookShowItemsPickedUp()
{
	detour_init(&showitemsctx, Sigs::CInventoryManager_ShowItemsPickedUp.GetPointer(),
		    (void *)&HookedShowItemsPickedUpFn);
	detour_enable(&showitemsctx);

#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("CInventoryManager::ShowItemsPickedUp hooked\n");
#endif
}