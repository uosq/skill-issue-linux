#include "../sdk/interfaces/interfaces.h"

#include "../hooks.h"

#include "../features/hook_initializer/initializer.h"

using GetUserCmdFn = CUserCmd *(*)(void *input, int sequence_number);

static CUserCmd* GetUserCmd(void *input, int sequence_number)
{
	return &(static_cast<IInput *>(interfaces::CInput)->m_pCommands[sequence_number % 90]);
}

static void Hook_GetUserCmd(void)
{
	VMTHooks::CInput.Hook(8, &GetUserCmd);

#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("Hooked CInput::GetUserCmd\n");

	#endif
}

MARK_FOR_INIT(Hook_GetUserCmd)