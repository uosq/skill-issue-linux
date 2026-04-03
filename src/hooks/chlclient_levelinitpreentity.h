#pragma once

#include "../features/entitylist/entitylist.h"
#include "../sdk/helpers/helper.h"
#include "../vtables.h"

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

static void AS_LevelInitPreEntity_Callback(const char *mapName)
{
	Hooks_CallHooks("LevelInitPreEntity", [&](asIScriptContext *ctx) { ctx->SetArgObject(0, &mapName); });
}

DECLARE_VTABLE_HOOK(LevelInitPreEntity, void, (CHLClient * thisptr, const char *mapName))
{
	AS_LevelInitPreEntity_Callback(mapName);
	originalLevelInitPreEntity(thisptr, mapName);
}

static void HookLevelInitPreEntity()
{
	INSTALL_VTABLE_HOOK(LevelInitPreEntity, interfaces::ClientDLL, 5);

#ifdef DEBUG
	constexpr Color_t color{100, 255, 100, 255};
	helper::console::ColoredPrint("BaseClientDll::LevelInitPreEntity hooked\n", color);
#endif
}