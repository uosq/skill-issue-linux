#pragma once

#include "../sdk/interfaces/interfaces.h"
#include "../sdk/classes/entity.h"
#include "../sdk/classes/player.h"
#include "../sdk/helpers/helper.h"
#include "../features/aimbot/aimbot.h"
#include <string>

// Source https://8dcc.github.io/reversing/reversing-tf2-bsendpacket.html#introduction
#define SENDPACKET_STACK_OFFSET 0xF8

using CreateMoveFn = bool (*)(IClientMode* thisptr, float sample_frametime, CUserCmd* pCmd);
inline CreateMoveFn originalCreateMove = nullptr;

inline bool HookedCreateMove (IClientMode* thisptr, float sample_frametime, CUserCmd* pCmd)
{
	uintptr_t current_frame_address = reinterpret_cast<uintptr_t>(__builtin_frame_address(0));
    	uintptr_t current_stack_address = current_frame_address + 0x8;
    	bool* bSendPacket = (bool*)(current_stack_address + SENDPACKET_STACK_OFFSET);

	if (!pCmd || !pCmd->command_number)
		return originalCreateMove(thisptr, sample_frametime, pCmd);

	if (!interfaces::engine->IsInGame() || !interfaces::engine->IsConnected())
		return originalCreateMove(thisptr, sample_frametime, pCmd);

	Vector originalAngles = pCmd->viewangles;

	// populate movement
	int ret = originalCreateMove(thisptr, sample_frametime, pCmd);

	CTFPlayer* pLocal = helper::engine::GetLocalPlayer();
	if (!pLocal || !pLocal->IsAlive())
		return ret;

	CTFWeaponBase* pWeapon = HandleAs<CTFWeaponBase>(pLocal->GetActiveWeapon());
	if (!pWeapon)
		return ret;

	Aimbot::Run(pLocal, pWeapon, pCmd, bSendPacket);
	helper::engine::FixMovement(pCmd, originalAngles, pCmd->viewangles);

	// Return false so the engine doesn't apply it to engine->SetViewAngles; (this is stupid)*/
	return false;
}

inline void HookCreateMove()
{
	void** vt = vtable::get(interfaces::clientMode);
	originalCreateMove = vtable::hook(vt, 22, &HookedCreateMove);

	helper::console::ColoredPrint("ClientModeShared::CreateMove hooked\n", (Color_t){100, 255, 100, 255});
}