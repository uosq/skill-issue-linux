#include "chlclient_levelshutdown.h"

DECLARE_VTABLE_HOOK(LevelShutdown, void, (CHLClient * thisptr))
{
	EntityList::Clear();
	Warp::Reset();
	Backtrack::Reset();
	gAimProjectile.Reset();
	gAimProjectile.ResetIndicator();
	Bhop::Reset();
	Spectators::OnLevelShutdown();

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