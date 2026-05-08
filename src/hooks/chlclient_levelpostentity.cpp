#include "chlclient_levelpostentity.h"

#include "../hooks.h"

#include "../features/entitylist/entitylist.h"
#include "../features/spectators/spectators.h"
#include "../features/visuals/viewmodel_aim/viewmodel_aim.h"
#include "../features/chams/chams.h"
#include "../features/esp/esp.h"

#include "../features/angelscript/api/libraries/hooks/hooks.h"

using LevelInitPostEntityFn = void (*)(CHLClient *thisptr);

static void LevelInitPostEntity(CHLClient* rdi)
{
	auto original = VMTHooks::Client.GetOriginal<LevelInitPostEntityFn>(6);
	original(rdi);

	EntityList::Reserve();
	ViewmodelAim::ResetStopTime();
	Spectators::OnLevelInitPostEntity();
	Chams::OnLevelPostEntity();
	ESP::OnlevelInitPostEntity();

	Hooks_CallHooks("LevelInitPostEntity");
}

void HookLevelInitPostEntity()
{
	VMTHooks::Client.Hook(6, &LevelInitPostEntity);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("BaseClientDll::LevelInitPostEntity hooked\n", color);
#endif
}