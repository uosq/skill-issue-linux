#include "../features/gui/gui.h"

#include "../features/MaterialManager/materialmanager.h"
#include "../features/scriptmanager/scriptmanager.h"
#include "../features/chams/chams.h"
#include "../features/playerlist/playerlist.h"
#include "../features/hookmanager/hookmanager.hpp"
#include "../features/network/network.h"

INIT_HOOK(Host_Shutdown, void, (), "engine.so", "80 3D ? ? ? ? 00 0F 85 ? ? ? ? 55 31 F6")
{
	features::chams.OnGameShutdown();
	features::playerlist.Shutdown();
	features::network.shutdown();
	features::gui.shutdown();
	features::materials.Uninitialize();

	features::scriptmanager.CallHooks("GameShutdown");

	original_Host_Shutdown();
}