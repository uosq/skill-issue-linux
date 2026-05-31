#include "../hooks.h"
#include "../core/core.h"

#include "../features/entitylist/entitylist.h"
#include "../features/spectators/spectators.h"
#include "../features/visuals/viewmodel_aim/viewmodel_aim.h"
#include "../features/chams/chams.h"
#include "../features/esp/esp.h"

#include "../features/scriptmanager/scriptmanager.h"
#include "../features/hook_initializer/initializer.h"

using LevelInitPostEntityFn = void (*)(CHLClient *thisptr);

static void LevelInitPostEntity(CHLClient* rdi)
{
	auto original = VMTHooks::Client.GetOriginal<LevelInitPostEntityFn>(6);
	original(rdi);

	if (!gApp->IsInitialized())
		return;

	features::entities.Reserve();
	features::viewmodel_aim.ResetStopTime();
	features::spectators.OnLevelInitPostEntity();
	features::chams.OnLevelPostEntity();
	features::esp.OnlevelInitPostEntity();

	features::scriptmanager.CallHooks("LevelInitPostEntity");
}

void HookLevelInitPostEntity()
{
	VMTHooks::Client.Hook(6, &LevelInitPostEntity);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("BaseClientDll::LevelInitPostEntity hooked\n", color);
#endif
}

MARK_FOR_INIT(HookLevelInitPostEntity)