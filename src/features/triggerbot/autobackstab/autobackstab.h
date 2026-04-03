#pragma once

#include "../../../sdk/classes/player.h"
#include "../../../sdk/classes/weaponbase.h"
#include "../../../sdk/definitions/cgametrace.h"
#include "../../../sdk/definitions/ctracefilters.h"
#include "../../../sdk/helpers/helper.h"
#include "../../../settings/settings.h"
#include "../../aimbot/utils/utils.h"

namespace AutoBackstab
{
	bool IsBehindEntity(CTFPlayer *pLocal, CTFPlayer *pTarget);
	bool IsBehindEntity(Vector localCenter, Vector targetCenter, Vector targetViewAngles);
	bool IsBehindAndFacingEntity(Vector localCenter, Vector targetCenter, Vector localViewAngles,
				     Vector targetViewAngles);
	bool IsBehindAndFacingEntity(CTFPlayer *pLocal, CTFPlayer *pTarget);
	bool CanBackstabEntity(CTFPlayer *pLocal, CTFPlayer *pTarget);

	void Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, bool *pSendPacket);
	std::string GetModeName();
}; // namespace AutoBackstab