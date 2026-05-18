#include "cinput_validadeusercmd.h"

#include "../libdetour/libdetour.h"

//DETOUR_DECL_TYPE(void, ValidateUserCmd, void *, CUserCmd *, int);

void Hooked_ValidateUserCmd(void *input, CUserCmd *pCmd, int sequence_number)
{
}

void Hook_ValidateUserCmd(void)
{
	detour_ctx_t validate_ctx;
	detour_init(&validate_ctx, Sigs::CInput_ValidateUserCmd.GetPointer(), (void *)&Hooked_ValidateUserCmd);

	if (!detour_enable(&validate_ctx))
	{
		interfaces::Cvar->ConsolePrintf("Failed to hook CInput::ValidateUserCmd!\n");
		return;
	}

#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("CInput::ValidateUserCmd hooked\n");
#endif
}