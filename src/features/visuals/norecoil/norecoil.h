#pragma once

#include "../../../sdk/classes/player.h"
#include "../../../sdk/classes/weaponbase.h"

#include "../../feature.h"
#include "../../config/config.h"

BEGIN_CONFIG(norecoil)

	ADD_CONFIG(enabled, "no recoil enabled", false)
	ADD_CONFIG(scale, "no recoil scale", 100)
	ADD_CONFIG(ignore_spectators, "no recoil ignore spectators", false)

END_CONFIG()

class NoRecoil
{
public:
	void RunOverrideView(CTFPlayer *pLocal, CViewSetup *pView);
	void RunCreateMove(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd);
	void RunCalcViewModelView(Vector& angle);

	float GetScale();
};

DECLARE_FEATURE(NoRecoil, norecoil)