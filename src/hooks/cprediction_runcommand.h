#pragma once

#include "../libdetour/libdetour.h"
#include "../sdk/classes/player.h"
#include "../sdk/definitions/cusercmd.h"
#include "../sdk/interfaces/interfaces.h"
#include "../sdk/signatures/signatures.h"

#include "../features/warp/warp.h"

ADD_SIG(CPrediction_RunCommand, "client.so",
	"55 48 89 E5 41 57 49 89 CF 41 56 41 55 49 89 D5 41 54 49 89 FC 53 "
	"48 89 F3")

// sig 55 48 89 E5 41 57 49 89 CF 41 56 41 55 49 89 D5 41 54 49 89 FC 53 48 89 F3
// args CPrediction *self,  C_BasePlayer *player, CUserCmd *ucmd, IMoveHelper *moveHelper
// xref: CPrediction::RunCommand

DETOUR_DECL_TYPE(void, RunCommand, IPrediction *self, CTFPlayer *player, CUserCmd *ucmd, void *moveHelper);
inline detour_ctx_t runcommand_ctx;

inline void Hooked_RunCommand(IPrediction *self, CTFPlayer *player, CUserCmd *ucmd, void *moveHelper)
{
	if (Warp::m_bShifting)
		player->GetTickBase() -= Warp::m_iShiftAmount;

	if (Warp::m_bRecharging)
		player->GetTickBase()--;

	DETOUR_ORIG_CALL(&runcommand_ctx, RunCommand, self, player, ucmd, moveHelper);
}

// I should probably just vtable hook it but oh well :p
inline void Hook_RunCommand(void)
{
	detour_init(&runcommand_ctx, Sigs::CPrediction_RunCommand.GetPointer(), (void *)&Hooked_RunCommand);
	if (!detour_enable(&runcommand_ctx))
	{
		interfaces::Cvar->ConsolePrintf("Failed to hook CPrediction::RunCommand\n");
		return;
	}

#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("Hooked CPrediction::RunCommand\n");
#endif
}