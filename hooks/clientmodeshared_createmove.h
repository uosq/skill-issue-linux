#pragma once

#include "../sdk/sdk.h"

using CreateMoveFn = bool (*)(IClientMode* thisptr, float sample_frametime, CUserCmd* pCmd);
inline CreateMoveFn originalCreateMove = nullptr;

inline bool HookedCreateMove (IClientMode* thisptr, float sample_frametime, CUserCmd* pCmd)
{
	if (!pCmd || !pCmd->command_number)
		return originalCreateMove(thisptr, sample_frametime, pCmd);
	originalCreateMove(thisptr, sample_frametime, pCmd);

	return false;
}

inline void HookCreateMove()
{
	void** vt = vtable::get(interfaces::clientMode);
	originalCreateMove = vtable::hook(vt, 22, &HookedCreateMove);

	interfaces::vstdlib->ConsolePrintf("Hooked ClientModeShared::CreateMove!\n");
}