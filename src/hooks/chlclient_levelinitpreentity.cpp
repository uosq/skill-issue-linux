#include "chlclient_levelinitpreentity.h"

DECLARE_VTABLE_HOOK(LevelInitPreEntity, void, (CHLClient * thisptr, const char *mapName))
{
	AS_LevelInitPreEntity_Callback(mapName);
	originalLevelInitPreEntity(thisptr, mapName);
}

void AS_LevelInitPreEntity_Callback(const char *mapName)
{
	Hooks_CallHooks("LevelInitPreEntity", [&](asIScriptContext *ctx) { ctx->SetArgObject(0, &mapName); });
}

void HookLevelInitPreEntity()
{
	INSTALL_VTABLE_HOOK(LevelInitPreEntity, interfaces::ClientDLL, 5);

#ifdef DEBUG
	constexpr Color_t color{100, 255, 100, 255};
	helper::console::ColoredPrint("BaseClientDll::LevelInitPreEntity hooked\n", color);
#endif
}