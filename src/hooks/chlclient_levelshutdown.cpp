#include "chlclient_levelshutdown.h"

#include "../vtables.h"

#include "../features/aimbot/projectile/projectile.h"
#include "../features/backtrack/backtrack.h"
#include "../features/bhop/bhop.h"
#include "../features/entitylist/entitylist.h"
#include "../features/spectators/spectators.h"
#include "../features/warp/warp.h"
#include "../features/chams/chams.h"
#include "../features/esp/esp.h"

#include "../features/angelscript/api/libraries/hooks/hooks.h"

DECLARE_VTABLE_HOOK(LevelShutdown, void, (CHLClient * thisptr))
{
	EntityList::Clear();
	Warp::Reset();
	Backtrack::Reset();
	gAimProjectile.Reset();
	gAimProjectile.ResetIndicator();
	Bhop::Reset();
	Spectators::OnLevelShutdown();
	Chams::OnLevelShutdown();
	ESP::OnLevelShutdown();

	Hooks_CallHooks("LevelShutdown");
	originalLevelShutdown(thisptr);
}

void HookLevelShutdown()
{
	INSTALL_VTABLE_HOOK(LevelShutdown, interfaces::ClientDLL, 7);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("BaseClientDll::LevelShutdown hooked\n", color);
#endif
}