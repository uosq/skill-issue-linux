#include "../sdk/definitions/ivengineclient.h"
#include "../sdk/interfaces/interfaces.h"

#include "../features/scriptmanager/scriptmanager.h"
#include "../features/hookmanager/hookmanager.hpp"

using LevelInitPreEntityFn = void(*)(CHLClient* thisptr, const char* mapName);

INIT_VTABLE_HOOK(LevelInitPreEntity, void, (CHLClient* thisptr, const char* mapName), interfaces::ClientDLL, 5)
{
	// lua callback
	features::scriptmanager.CallHooks("LevelInitPreEntity", &mapName);

	original_LevelInitPreEntity(thisptr, mapName);
}