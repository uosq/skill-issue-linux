#pragma once

#include "../../sdk/classes/player.h"
#include "../../sdk/classes/weaponbase.h"
#include "../../sdk/definitions/cgametrace.h"
#include "../../sdk/definitions/ctracefilters.h"
#include "../../sdk/helpers/helper.h"

#include "../../settings/settings.h"
#include "../aimbot/utils/utils.h"

#include "autoairblast/autoairblast.h"
#include "autobackstab/autobackstab.h"

#include "../feature.h"

class Triggerbot
{
public:
	void Hitscan(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd);
	void Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd);
};

DECLARE_FEATURE(Triggerbot, trigger)