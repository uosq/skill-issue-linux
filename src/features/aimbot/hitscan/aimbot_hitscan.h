#pragma once

#include "../../../sdk/definitions/cusercmd.h"
#include "../../../sdk/classes/player.h"
#include "../../../sdk/classes/weaponbase.h"
#include "../../../sdk/helpers/helper.h"
#include "../../../sdk/handle_utils.h"
#include "../../../sdk/definitions/ienginetrace.h"
#include "../../../sdk/definitions/cgametrace.h"
#include "../../../sdk/definitions/ctracefilters.h"
#include "../../../sdk/definitions/bspflags.h"
#include "../../../settings/settings.h"
#include "../../../sdk/definitions/studio.h"

#include "../utils/utils.h"
#include "../../entitylist/entitylist.h"

#include <cmath>
#include <vector>

enum class HitscanOffset
{
	HEAD = 0,
	CHEST,
	//PELVIS
	// legs are unreliable
};

namespace AimbotHitscan
{
	HitscanOffset GetInitialOffset(CTFPlayer* pLocal, CTFWeaponBase* pWeapon);
	bool GetShotPosition(CTFPlayer* pLocal, CBaseEntity* pTarget, CTFWeaponBase* pWeapon, Vector eyePos, Vector& shotPosition);
	void Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, AimbotState& state);
};