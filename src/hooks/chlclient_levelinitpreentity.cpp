#include "../core/core.h"

#include "../hooks.h"

#include "../sdk/definitions/ivengineclient.h"

#include "../features/scriptmanager/scriptmanager.h"
#include "../features/hook_initializer/initializer.h"

using LevelInitPreEntityFn = void(*)(CHLClient* thisptr, const char* mapName);

static void LevelInitPreEntity(CHLClient* thisptr, const char* mapName)
{
	auto original = VMTHooks::Client.GetOriginal<LevelInitPreEntityFn>(5);

	if (gApp->IsInitialized())
	{
		// lua callback
		features::scriptmanager.CallHooks("LevelInitPreEntity", &mapName);
	}

	original(thisptr, mapName);
}

void HookLevelInitPreEntity()
{
	VMTHooks::Client.Hook(5, &LevelInitPreEntity);

#ifdef DEBUG
	constexpr Color_t color{100, 255, 100, 255};
	helper::console::ColoredPrint("BaseClientDll::LevelInitPreEntity hooked\n", color);
#endif
}

MARK_FOR_INIT(HookLevelInitPreEntity)