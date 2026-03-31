#pragma once

#include "../vtables.h"
#include "../sdk/helpers/helper.h"
#include "../features/entitylist/entitylist.h"
#include "../features/visuals/viewmodel_aim/viewmodel_aim.h"

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

DECLARE_VTABLE_HOOK(LevelInitPostEntity, void, (CHLClient* thisptr))
{
	EntityList::Reserve();
	ViewmodelAim::ResetStopTime();

	Hooks_CallHooks("LevelInitPostEntity");
	originalLevelInitPostEntity(thisptr);
}

static void HookLevelInitPostEntity()
{
	INSTALL_VTABLE_HOOK(LevelInitPostEntity, interfaces::ClientDLL, 6);

	#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("BaseClientDll::LevelInitPostEntity hooked\n", color);
	#endif
}