#include "../features/entitylist/entitylist.h"
#include "../features/spectators/spectators.h"
#include "../features/visuals/viewmodel_aim/viewmodel_aim.h"
#include "../features/chams/chams.h"
#include "../features/esp/esp.h"

#include "../features/scriptmanager/scriptmanager.h"
#include "../features/hookmanager/hookmanager.hpp"

using LevelInitPostEntityFn = void (*)(CHLClient *thisptr);

INIT_VTABLE_HOOK(LevelInitPostEntity, void, (CHLClient* rdi), interfaces::ClientDLL, 6)
{
	original_LevelInitPostEntity(rdi);

	features::entities.Reserve();
	features::viewmodel_aim.ResetStopTime();
	features::spectators.OnLevelInitPostEntity();
	features::chams.OnLevelPostEntity();
	features::esp.OnlevelInitPostEntity();

	features::scriptmanager.CallHooks("LevelInitPostEntity");
}