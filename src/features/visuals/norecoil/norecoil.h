#pragma once

#include "../../../sdk/classes/player.h"
#include "../../../sdk/classes/weaponbase.h"

#include "../../feature.h"

class NoRecoil
{
public:
	void RunOverrideView(CTFPlayer *pLocal, CViewSetup *pView);
	void RunCreateMove(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd);
	void RunCalcViewModelView(Vector& angle);
};

DECLARE_FEATURE(NoRecoil, norecoil)