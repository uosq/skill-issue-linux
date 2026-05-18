#pragma once

#include "../sdk/signatures/signatures.h"

ADD_SIG(CInventoryManager_ShowItemsPickedUp, "client.so", "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 48 48 8B 07")

bool HookedShowItemsPickedUpFn(void *thisptr, bool bForce, bool bReturnToGame, bool bNoPanel);
void HookShowItemsPickedUp();