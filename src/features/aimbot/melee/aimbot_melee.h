#pragma once

#include <climits>

#include "../../../sdk/classes/player.h"
#include "../../../sdk/classes/weaponbase.h"
#include "../../../sdk/definitions/bspflags.h"
#include "../../../sdk/definitions/cgametrace.h"
#include "../../../sdk/definitions/ctracefilters.h"
#include "../../../sdk/definitions/cusercmd.h"
#include "../../../sdk/definitions/ienginetrace.h"
#include "../../../sdk/definitions/studio.h"
#include "../../../sdk/handle_utils.h"
#include "../../../sdk/helpers/helper.h"

#include "../../config/config.h"
#include "../../entitylist/entitylist.h"

#include "../utils/utils.h"

enum class MeleeMode
{
	NONE = 0,
	LEGIT,
	RAGE
};

class AimbotMelee
{
public:
	void Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, AimbotState &state);
};
