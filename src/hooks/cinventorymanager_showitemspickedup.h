#pragma once

#include <string>

#include "../libdetour/libdetour.h"
#include "../settings/settings.h"

#include "../sdk/classes/entity.h"
#include "../sdk/classes/player.h"
#include "../sdk/helpers/helper.h"
#include "../sdk/interfaces/interfaces.h"
#include "../sdk/signatures/signatures.h"

#include "../features/aimbot/aimbot.h"

ADD_SIG(CInventoryManager_ShowItemsPickedUp, "client.so", "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 48 48 8B 07")

bool HookedShowItemsPickedUpFn(void *thisptr, bool bForce, bool bReturnToGame, bool bNoPanel);
void HookShowItemsPickedUp();