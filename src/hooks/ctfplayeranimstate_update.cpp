#include "../sdk/classes/player.h"

#include "../features/entitylist/entitylist.h"
#include "../features/hookmanager/hookmanager.hpp"

// CMultiPlayerAnimState::Update
// one of them is it

INIT_HOOK(CTFPlayerAnimState_Update, void, (void* thisptr, float eyeYaw, float eyePitch), "client.so", "55 31 C0 48 89 E5 41 57 41 56 41 55 66 41 0F 7E CD")
{
	if (thisptr == nullptr)
		return;

	uintptr_t ptr = reinterpret_cast<uintptr_t>(thisptr);
	CTFPlayer* pPlayer = reinterpret_cast<CTFPlayer*>(ptr + 0x130);

	if (pPlayer == nullptr)
	{
		original_CTFPlayerAnimState_Update(thisptr, eyeYaw, eyePitch);
		return;
	}

	if (features::entities.GetLocal() == nullptr)
	{
		original_CTFPlayerAnimState_Update(thisptr, eyeYaw, eyePitch);
		return;
	}

	if (pPlayer != features::entities.GetLocal())
	{
		original_CTFPlayerAnimState_Update(thisptr, eyeYaw, eyePitch);
		return;
	}

	// this shit does nothing
	// am i doing something wrong?
	// perhaps i am not writing to the right place??
	// on ghidra this has no parameters, so maybe that is why?
	original_CTFPlayerAnimState_Update(thisptr, helper::localplayer::LastAngle.y, helper::localplayer::LastAngle.x);
}
