#pragma once

#include "../../sdk/classes/player.h"
#include "../../sdk/classes/weaponbase.h"
#include "../../sdk/definitions/cgametrace.h"
#include "../../sdk/definitions/ctracefilters.h"
#include "../../sdk/helpers/helper.h"

#include "../config/config.h"
#include "../aimbot/utils/utils.h"

#include "autoairblast/autoairblast.h"
#include "autobackstab/autobackstab.h"

#include "../feature.h"

enum class GenericMode
{
	NONE = 0,
	LEGIT, RAGE
};

BEGIN_CONFIG(trigger)

	ADD_CONFIG(enabled, "trigger enabled", false)
	ADD_CONFIG(key, "trigger key", Hotkey{})
	ADD_CONFIG(hitscan, "trigger hitscan", false)

END_CONFIG()

class Triggerbot
{
public:
	void Hitscan(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd);
	void Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd);
};

DECLARE_FEATURE(Triggerbot, trigger)