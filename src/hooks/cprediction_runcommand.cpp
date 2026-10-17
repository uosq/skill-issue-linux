#include "../sdk/classes/player.h"
#include "../sdk/definitions/cusercmd.h"

#include "../features/warp/warp.h"
#include "../features/hookmanager/hookmanager.hpp"

// sig 55 48 89 E5 41 57 49 89 CF 41 56 41 55 49 89 D5 41 54 49 89 FC 53 48 89 F3
// args CPrediction *self,  C_BasePlayer *player, CUserCmd *ucmd, IMoveHelper *moveHelper
// xref: CPrediction::RunCommand

INIT_HOOK(IPrediction_RunCommand, void, (IPrediction *self, CTFPlayer *player, CUserCmd *ucmd, void *moveHelper), "client.so", "55 48 89 E5 41 57 49 89 CF 41 56 41 55 49 89 D5 41 54 49 89 FC 53 48 89 F3")
{
	if (features::warp.IsShifting())
		player->GetTickBase() -= features::warp.m_iShiftAmount;

	if (features::warp.IsRecharging())
		player->GetTickBase()--;

	original_IPrediction_RunCommand(self, player, ucmd, moveHelper);
}