#include "cprediction_runcommand.h"

DETOUR_DECL_TYPE(void, RunCommand, IPrediction *self, CTFPlayer *player, CUserCmd *ucmd, void *moveHelper);
detour_ctx_t runcommand_ctx;

void Hooked_RunCommand(IPrediction *self, CTFPlayer *player, CUserCmd *ucmd, void *moveHelper)
{
	if (Warp::m_bShifting)
		player->GetTickBase() -= Warp::m_iShiftAmount;

	if (Warp::m_bRecharging)
		player->GetTickBase()--;

	DETOUR_ORIG_CALL(&runcommand_ctx, RunCommand, self, player, ucmd, moveHelper);
}

// I should probably just vtable hook it but oh well :p
void Hook_RunCommand(void)
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