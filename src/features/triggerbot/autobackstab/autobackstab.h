#pragma once

#include "../../../sdk/classes/player.h"
#include "../../../sdk/classes/weaponbase.h"
#include "../../../sdk/definitions/cgametrace.h"
#include "../../../sdk/definitions/ctracefilters.h"
#include "../../../sdk/helpers/helper.h"

#include "../../config/config.h"
#include "../../aimbot/utils/utils.h"

#include "../../feature.h"

BEGIN_CONFIG(autobackstab)

	ADD_CONFIG(enabled, "trigger auto backstab enabled", 0)

END_CONFIG()

class AutoBackstab
{
public:
	bool IsBehindEntity(CTFPlayer *pLocal, CTFPlayer *pTarget);
	bool IsBehindEntity(Vector localCenter, Vector targetCenter, Vector targetViewAngles);
	bool IsBehindAndFacingEntity(Vector localCenter, Vector targetCenter, Vector localViewAngles,
				     Vector targetViewAngles);
	bool IsBehindAndFacingEntity(CTFPlayer *pLocal, CTFPlayer *pTarget);
	bool CanBackstabEntity(CTFPlayer *pLocal, CTFPlayer *pTarget);

	void Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, bool *pSendPacket);
};

DECLARE_FEATURE(AutoBackstab, autobackstab)