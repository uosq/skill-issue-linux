#include "chlclient_levelpostentity.h"

#include "../vtables.h"

#include "../features/entitylist/entitylist.h"
#include "../features/spectators/spectators.h"
#include "../features/visuals/viewmodel_aim/viewmodel_aim.h"
#include "../features/chams/chams.h"
#include "../features/esp/esp.h"

#include "../features/angelscript/api/libraries/hooks/hooks.h"

DECLARE_VTABLE_HOOK(LevelInitPostEntity, void, (CHLClient * thisptr))
{
	EntityList::Reserve();
	ViewmodelAim::ResetStopTime();
	Spectators::OnLevelInitPostEntity();
	Chams::OnLevelPostEntity();
	ESP::OnlevelInitPostEntity();

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