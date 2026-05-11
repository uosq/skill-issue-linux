#include "chlclient_levelshutdown.h"

#include "../hooks.h"

#include "../features/aimbot/projectile/projectile.h"
#include "../features/backtrack/backtrack.h"
#include "../features/bhop/bhop.h"
#include "../features/entitylist/entitylist.h"
#include "../features/spectators/spectators.h"
#include "../features/warp/warp.h"
#include "../features/chams/chams.h"
#include "../features/esp/esp.h"
#include "../features/antiafk/antiafk.h"

#include "../features/scriptmanager/scriptmanager.h"

using LevelShutdownFn = void (*)(CHLClient* rdi);

static void LevelShutdown(CHLClient* rdi)
{
	features::entities.Clear();
	features::warp.Reset();
	features::backtrack.Reset();
	gAimProjectile.Reset();
	gAimProjectile.ResetIndicator();
	features::bhop.Reset();
	features::spectators.OnLevelShutdown();
	features::chams.OnLevelShutdown();
	features::esp.OnLevelShutdown();
	features::antiafk.OnLevelShutdown();

	features::scriptmanager.CallHooks("LevelShutdown");

	auto original = VMTHooks::Client.GetOriginal<LevelShutdownFn>(7);
	original(rdi);
}

void HookLevelShutdown()
{
	VMTHooks::Client.Hook(7, &LevelShutdown);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("BaseClientDll::LevelShutdown hooked\n", color);
#endif
}