#include "../sdk/definitions/igameevents.h"

#include "../features/scriptmanager/scriptmanager.h"
#include "../features/hookmanager/hookmanager.hpp"

// xref: #game_player_joined_game
INIT_HOOK(FireGameEvent, void, (void* self, IGameEvent* event), "client.so", "55 48 89 E5 41 57 41 56 49 89 FE 41 55 41 54 49 89 F4 53 48 8D 35")
{
	features::scriptmanager.CallHooks("FireGameEvent", event);
	original_FireGameEvent(self, event);
}