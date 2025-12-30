#pragma once

#include "../sdk/interfaces/interfaces.h"
#include "../sdk/classes/entity.h"
#include "../sdk/classes/player.h"
#include "../sdk/helpers/helper.h"
#include <string>

using CreateMoveFn = bool (*)(IClientMode* thisptr, float sample_frametime, CUserCmd* pCmd);
inline CreateMoveFn originalCreateMove = nullptr;

inline bool HookedCreateMove (IClientMode* thisptr, float sample_frametime, CUserCmd* pCmd)
{
	if (!pCmd || !pCmd->command_number)
		return originalCreateMove(thisptr, sample_frametime, pCmd);

	Vector originalAngles = pCmd->viewangles;

	// populate movement
	int ret = originalCreateMove(thisptr, sample_frametime, pCmd);

	/*Vector targetAngles = Vector(0, 0, 0);
	pCmd->viewangles = targetAngles;
	
	helper::engine::FixMovement(pCmd, originalAngles, targetAngles);
	// Return false so the engine doesn't apply it to engine->SetViewAngles; (this is stupid)*/
	return false;
}

inline void HookCreateMove()
{
	void** vt = vtable::get(interfaces::clientMode);
	originalCreateMove = vtable::hook(vt, 22, &HookedCreateMove);

	helper::console::ColoredPrint("ClientModeShared::CreateMove hooked\n", (Color_t){100, 255, 100, 255});
}