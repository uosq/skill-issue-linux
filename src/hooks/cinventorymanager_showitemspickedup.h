#pragma once

#include "../sdk/interfaces/interfaces.h"
#include "../sdk/classes/entity.h"
#include "../sdk/classes/player.h"
#include "../sdk/helpers/helper.h"
#include "../settings.h"
#include <string>

#include "../features/aimbot/aimbot.h"

#include "../libdetour/libdetour.h"

inline detour_ctx_t showitemsctx;
DETOUR_DECL_TYPE(bool, original_ShowItemsPickedUpFn, void* thisptr, bool bForce, bool bReturnToGame, bool bNoPanel);

static bool HookedShowItemsPickedUpFn(void* thisptr, bool bForce, bool bReturnToGame, bool bNoPanel)
{
	if (Settings::misc.accept_item_drop)
	{
		interfaces::Cvar->ConsolePrintf("Collected item drop\n");
		DETOUR_ORIG_CALL(&showitemsctx, original_ShowItemsPickedUpFn, thisptr, true, true, true);
		return false;
	}

	bool ret;
	DETOUR_ORIG_GET(&showitemsctx, ret, original_ShowItemsPickedUpFn, thisptr, bForce, bReturnToGame, bNoPanel);
	return ret;
}

static void HookShowItemsPickedUp()
{
	void* original = sigscan_module("client.so", "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 48 48 8B 07");
	detour_init(&showitemsctx, original, (void*)&HookedShowItemsPickedUpFn);
	detour_enable(&showitemsctx);

	interfaces::Cvar->ConsolePrintf("CInventoryManager::ShowItemsPickedUp hooked\n");
}