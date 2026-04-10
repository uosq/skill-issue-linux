#include "ctfplayeranimstate_update.h"

#include "../features/logs/logs.h"

DETOUR_DECL_TYPE(void, CTFPlayerAnimState_Update, void* thisptr, float eyeYaw, float eyePitch);
detour_ctx_t animstate_update_ctx;

void Hooked_CTFPlayerAnimState_Update(void* thisptr, float eyeYaw, float eyePitch)
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

	// this shit does nothing
	// am i doing something wrong?
	// perhaps i am not writing to the right place??
	// on ghidra this has no parameters, so maybe that is why?
	DETOUR_ORIG_CALL(&animstate_update_ctx, CTFPlayerAnimState_Update, thisptr, helper::localplayer::LastAngle.y, helper::localplayer::LastAngle.x);
}

void Hook_CTFPlayerAnimState_Update()
{
	detour_init(&animstate_update_ctx, Sigs::CTFPlayerAnimState_Update.GetPointer(), (void*)&Hooked_CTFPlayerAnimState_Update);
	if (!detour_enable(&animstate_update_ctx))
		Logs::Error("Couldn't hook CTFPlayerAnimState::Update");
}