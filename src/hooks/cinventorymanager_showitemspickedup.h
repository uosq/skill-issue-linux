#pragma once

#include "../sdk/interfaces/interfaces.h"
#include "../sdk/classes/entity.h"
#include "../sdk/classes/player.h"
#include "../sdk/helpers/helper.h"
#include "../settings.h"
#include <string>

#include "../features/aimbot/aimbot.h"

#include "../libdetour/libdetour.h"

using ShowItemsPickedUpFn = bool(*)(void* thisptr, bool bForce, bool bReturnToGame, bool bNoPanel);
static ShowItemsPickedUpFn originalShowItemsPickedUpFn = nullptr;

DETOUR_DECL_TYPE(bool, originalShowItemsPickedUpFn, void* thisptr, bool bForce, bool bReturnToGame, bool bNoPanel);

static detour_ctx_t showitemsctx;

static bool HookedShowItemsPickedUpFn(void* thisptr, bool bForce, bool bReturnToGame, bool bNoPanel)
{
	if (g_Settings.misc.accept_item_drop)
	{
		interfaces::Cvar->ConsolePrintf("Collected item drop\n");
		DETOUR_ORIG_CALL(&showitemsctx, originalShowItemsPickedUpFn, thisptr, true, true, true);
		return false;
	}

	bool ret;
	DETOUR_ORIG_GET(&showitemsctx, ret, originalShowItemsPickedUpFn, thisptr, bForce, bReturnToGame, bNoPanel);
	return ret;
}

static void HookShowItemsPickedUp()
{
	originalShowItemsPickedUpFn = reinterpret_cast<ShowItemsPickedUpFn>(sigscan_module("client.so", "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 48 48 8B 07"));
	detour_init(&showitemsctx, (void*)originalShowItemsPickedUpFn, (void*)&HookedShowItemsPickedUpFn);
	detour_enable(&showitemsctx);

	interfaces::Cvar->ConsolePrintf("CInventoryManager::ShowItemsPickedUp hooked\n");
}