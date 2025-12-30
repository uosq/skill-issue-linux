#pragma once

#include "../sdk/sdk.h"
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
	originalCreateMove(thisptr, sample_frametime, pCmd);

	Player* pLocal = helper::engine::GetLocalPlayer();
	helper::console::Print((std::to_string(pLocal->m_iTeamNum()) + "\n").c_str());

	/*Vector targetAngles = Vector(0, 0, 0);
	pCmd->viewangles = targetAngles;

	SDK::FixMovement(pCmd, originalAngles, targetAngles);*/
	// Return false so the engine doesn't apply it to engine->SetViewAngles; (this is stupid)
	return false;
}

inline void HookCreateMove()
{
	void** vt = vtable::get(interfaces::clientMode);
	originalCreateMove = vtable::hook(vt, 22, &HookedCreateMove);

	helper::console::ColoredPrint("ClientModeShared::CreateMove hooked\n", (Color_t){100, 255, 100, 255});
}