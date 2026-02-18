#pragma once

#include "../../../settings/settings.h"
#include "../../../sdk/classes/player.h"
#include "../../../sdk/classes/weaponbase.h"
#include "../../../sdk/helpers/helper.h"
#include "../../../sdk/definitions/cgametrace.h"
#include "../../../sdk/definitions/ctracefilters.h"
#include "../../aimbot/utils/utils.h"

namespace AutoBackstab
{
	bool IsBehindEntity(CTFPlayer* pLocal, CTFPlayer* pTarget);
	bool IsBehindAndFacingEntity(CTFPlayer *pLocal, CTFPlayer* pTarget);
	bool CanBackstabEntity(CTFPlayer* pLocal, CTFPlayer* pTarget);

	void Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, bool* pSendPacket);
	std::string GetModeName();
};