#pragma once

#include "../../../sdk/helpers/helper.h"
#include "../../../settings.h"

namespace NoRecoil
{
	void RunOverrideView(CTFPlayer* pLocal, CViewSetup* pView);
	void RunCreateMove(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd);
}