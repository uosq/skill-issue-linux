#include "../features/aimbot/aimbot.h"
#include "../features/backtrack/backtrack.h"
#include "../features/bhop/bhop.h"
#include "../features/entitylist/entitylist.h"
#include "../features/spectators/spectators.h"
#include "../features/warp/warp.h"
#include "../features/chams/chams.h"
#include "../features/esp/esp.h"
#include "../features/antiafk/antiafk.h"

#include "../features/scriptmanager/scriptmanager.h"
#include "../features/hookmanager/hookmanager.hpp"

using LevelShutdownFn = void (*)(CHLClient* rdi);

INIT_VTABLE_HOOK(LevelShutdown, void, (CHLClient* rdi), interfaces::ClientDLL, 7)
{
	features::entities.Clear();
	features::warp.Reset();
	features::backtrack.Reset();
	features::aimbot.Reset();
	features::bhop.Reset();
	features::spectators.OnLevelShutdown();
	features::chams.OnLevelShutdown();
	features::esp.OnLevelShutdown();
	features::antiafk.OnLevelShutdown();

	features::scriptmanager.CallHooks("LevelShutdown");

	original_LevelShutdown(rdi);
}