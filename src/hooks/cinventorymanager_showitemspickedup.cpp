#include "../core/core.h"

#include "../sdk/interfaces/interfaces.h"
#include "../sdk/signatures/signatures.h"

#include "../thirdparty/libdetour/libdetour.h"

#include "../features/misc/misc.h"
#include "../features/hook_initializer/initializer.h"

ADD_SIG(CInventoryManager_ShowItemsPickedUp, "client.so", "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 48 48 8B 07")

DETOUR_DECL_TYPE(bool, original_ShowItemsPickedUpFn, void *thisptr, bool bForce, bool bReturnToGame, bool bNoPanel);
detour_ctx_t showitemsctx;

bool HookedShowItemsPickedUpFn(void *thisptr, bool bForce, bool bReturnToGame, bool bNoPanel)
{
	if (gApp->IsInitialized())
	{
		if (config::accept_item_drop::enabled.Get())
		{
			interfaces::Cvar->ConsolePrintf("Collected item drop\n");
			DETOUR_ORIG_CALL(&showitemsctx, original_ShowItemsPickedUpFn, thisptr, true, true, true);
			return false;
		}
	}

	bool ret {};
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

MARK_FOR_INIT(HookShowItemsPickedUp)