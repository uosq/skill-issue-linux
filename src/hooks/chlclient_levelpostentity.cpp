#include "chlclient_levelpostentity.h"

DECLARE_VTABLE_HOOK(LevelInitPostEntity, void, (CHLClient * thisptr))
{
	EntityList::Reserve();
	ViewmodelAim::ResetStopTime();
	Spectators::OnLevelInitPostEntity();

	Hooks_CallHooks("LevelInitPostEntity");
	originalLevelInitPostEntity(thisptr);
}

void HookLevelInitPostEntity()
{
	INSTALL_VTABLE_HOOK(LevelInitPostEntity, interfaces::ClientDLL, 6);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("BaseClientDll::LevelInitPostEntity hooked\n", color);
#endif
}