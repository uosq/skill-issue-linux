#include "cinput_getusercmd.h"

#include "../sdk/interfaces/interfaces.h"

#include "../hooks.h"

using GetUserCmdFn = CUserCmd *(*)(void *input, int sequence_number);

static CUserCmd* GetUserCmd(void *input, int sequence_number)
{
	return &(static_cast<IInput *>(interfaces::CInput)->m_pCommands[sequence_number % 90]);
}

void Hook_GetUserCmd(void)
{
	VMTHooks::CInput.Hook(8, &GetUserCmd);

#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("Hooked CInput::GetUserCmd\n");

	#endif
}