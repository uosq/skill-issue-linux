#pragma once

#include "../vtables.h"
#include "../sdk/helpers/helper.h"
#include "../features/entitylist/entitylist.h"

DECLARE_VTABLE_HOOK(LevelShutdown, void, (IBaseClientDLL* thisptr))
{
	EntityList::Clear();

	originalLevelShutdown(thisptr);
}

static void HookLevelShutdown()
{
	INSTALL_VTABLE_HOOK(LevelShutdown, interfaces::BaseClientDLL, 7);
	helper::console::ColoredPrint("BaseClientDll::LevelShutdown hooked\n", (Color_t){100, 255, 100, 255});
}