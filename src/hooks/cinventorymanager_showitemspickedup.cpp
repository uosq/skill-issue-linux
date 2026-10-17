#include "../sdk/interfaces/interfaces.h"

#include "../features/misc/misc.h"
#include "../features/hookmanager/hookmanager.hpp"

INIT_HOOK(ShowItemsPickedUp, bool, (void *thisptr, bool bForce, bool bReturnToGame, bool bNoPanel), "client.so", "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 48 48 8B 07")
{
	if (config::accept_item_drop::enabled.Get())
	{
		interfaces::Cvar->ConsolePrintf("Collected item drop\n");
		original_ShowItemsPickedUp(thisptr, true, true, true);
		return false;
	}

	return original_ShowItemsPickedUp(thisptr, bForce, bReturnToGame, bNoPanel);
}