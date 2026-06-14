#pragma once

#include "../../../sdk/classes/player.h"
#include "../../../sdk/classes/weaponbase.h"
#include "../../../sdk/definitions/cgametrace.h"
#include "../../../sdk/definitions/ctracefilters.h"
#include "../../../sdk/definitions/cusercmd.h"
#include "../../../sdk/helpers/helper.h"

#include "../../config/config.h"
#include "../../entitylist/entitylist.h"

#include "../../feature.h"

BEGIN_CONFIG(autoairblast)

	ADD_CONFIG(enabled, "trigger auto airblast enabled", 0)

END_CONFIG()

class AutoAirblast
{
public:
	// void Legit(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd*
	// pCmd); void Rage(CTFPlayer* pLocal, CTFWeaponBase* pWeapon,
	// CUserCmd* pCmd, bool* pSendPacket);

	void Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, bool *pSendPacket);
	bool CanAirblastHit(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CBaseEntity *pTarget, Vector &vecForward);
};

DECLARE_FEATURE(AutoAirblast, autoairblast)