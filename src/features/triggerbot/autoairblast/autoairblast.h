#pragma once

#include "../../../sdk/classes/player.h"
#include "../../../sdk/classes/weaponbase.h"
#include "../../../sdk/definitions/cusercmd.h"
#include "../../../sdk/definitions/cgametrace.h"
#include "../../../sdk/definitions/ctracefilters.h"
#include "../../../sdk/helpers/helper.h"
#include "../../entitylist/entitylist.h"
#include "../../../settings/settings.h"
#include <bits/types/locale_t.h>

namespace AutoAirblast
{
	//void Legit(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd);
	//void Rage(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, bool* pSendPacket);

	void Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, bool* pSendPacket);
	bool CanAirblastHit(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CBaseEntity* pTarget, Vector& vecForward);
	std::string GetModeName();
};