#pragma once

#include "../features/lua/hookmgr.h"
#include "../features/lua/api.h"
#include "../features/lua/classes.h"
#include "../vtables.h"

DECLARE_VTABLE_HOOK(GetUserCmd, CUserCmd*, (void* input, int sequence_number))
{
	return &(static_cast<IInput*>(interfaces::CInput)->m_pCommands[sequence_number % 90]);
}

inline void Hook_GetUserCmd(void)
{
	INSTALL_VTABLE_HOOK(GetUserCmd, interfaces::CInput, 8);
	interfaces::Cvar->ConsolePrintf("Hooked CInput::GetUserCmd\n");
}