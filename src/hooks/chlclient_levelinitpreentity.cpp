#include "chlclient_levelinitpreentity.h"

#include "../sdk/definitions/ivengineclient.h"

#include "../hooks.h"

#include "../features/angelscript/api/libraries/hooks/hooks.h"

using LevelInitPreEntityFn = void(*)(CHLClient* thisptr, const char* mapName);

static void LevelInitPreEntity(CHLClient* thisptr, const char* mapName)
{
	auto original = VMTHooks::Client.GetOriginal<LevelInitPreEntityFn>(5);

	AS_LevelInitPreEntity_Callback(mapName);
	original(thisptr, mapName);
}

void AS_LevelInitPreEntity_Callback(const char *mapName)
{
	Hooks_CallHooks("LevelInitPreEntity", [&](asIScriptContext* ctx) { ctx->SetArgObject(0, &mapName); });
}

void HookLevelInitPreEntity()
{
	VMTHooks::Client.Hook(5, &LevelInitPreEntity);

#ifdef DEBUG
	constexpr Color_t color{100, 255, 100, 255};
	helper::console::ColoredPrint("BaseClientDll::LevelInitPreEntity hooked\n", color);
#endif
}