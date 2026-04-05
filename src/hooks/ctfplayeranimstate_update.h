#pragma once

#include "../sdk/signatures/signatures.h"
#include "../sdk/classes/player.h"

#include "../libdetour/libdetour.h"
#include "../libsigscan.h"

#include "../features/entitylist/entitylist.h"
#include "../features/ticks/ticks.h"

// CMultiPlayerAnimState::Update
// one of them is it
ADD_SIG(CTFPlayerAnimState_Update, "client.so", "55 31 C0 48 89 E5 41 57 41 56 41 55 66 41 0F 7E CD")
DETOUR_DECL_TYPE(void, CTFPlayerAnimState_Update, void* thisptr, float eyeYaw, float eyePitch);
static detour_ctx_t animstate_update_ctx;

static void Hooked_CTFPlayerAnimState_Update(void* thisptr, float eyeYaw, float eyePitch)
{
	if (thisptr == nullptr)
		return;

	uintptr_t ptr = reinterpret_cast<uintptr_t>(thisptr);
	CTFPlayer* pPlayer = reinterpret_cast<CTFPlayer*>(ptr + 0x130);

	if (pPlayer == nullptr)
	{
		DETOUR_ORIG_CALL(&animstate_update_ctx, CTFPlayerAnimState_Update, thisptr, eyeYaw, eyePitch);
		return;
	}

	if (EntityList::GetLocal() == nullptr)
	{
		DETOUR_ORIG_CALL(&animstate_update_ctx, CTFPlayerAnimState_Update, thisptr, eyeYaw, eyePitch);
		return;
	}

	if (pPlayer != EntityList::GetLocal())
	{
		DETOUR_ORIG_CALL(&animstate_update_ctx, CTFPlayerAnimState_Update, thisptr, eyeYaw, eyePitch);
		return;
	}

	DETOUR_ORIG_CALL(&animstate_update_ctx, CTFPlayerAnimState_Update, thisptr, helper::localplayer::LastAngle.y, helper::localplayer::LastAngle.x);
}