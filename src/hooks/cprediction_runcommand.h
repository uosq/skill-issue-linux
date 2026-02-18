#pragma once

#include "../sdk/interfaces/interfaces.h"
#include "../sdk/classes/player.h"
#include "../sdk/definitions/cusercmd.h"
#include "../libdetour/libdetour.h"

#include "../features/warp/warp.h"

// sig 55 48 89 E5 41 57 49 89 CF 41 56 41 55 49 89 D5 41 54 49 89 FC 53 48 89 F3
// args CPrediction *self,  C_BasePlayer *player, CUserCmd *ucmd, IMoveHelper *moveHelper

DETOUR_DECL_TYPE(void, RunCommand, IPrediction* self, CTFPlayer* player, CUserCmd* ucmd, void* moveHelper);
inline detour_ctx_t runcommand_ctx;

inline void Hooked_RunCommand(IPrediction* self, CTFPlayer* player, CUserCmd* ucmd, void* moveHelper)
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
	// xref: CPrediction::RunCommand
	void* original = sigscan_module("client.so", "55 48 89 E5 41 57 49 89 CF 41 56 41 55 49 89 D5 41 54 49 89 FC 53 48 89 F3");

	detour_init(&runcommand_ctx, original, (void*)&Hooked_RunCommand);
	if (!detour_enable(&runcommand_ctx))
	{
		interfaces::Cvar->ConsolePrintf("Failed to hook CPrediction::RunCommand\n");
		return;
	}

	#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("Hooked CPrediction::RunCommand\n");
	#endif
}