#pragma once

#include "../../sdk/classes/player.h"
#include "../../sdk/classes/weaponbase.h"
#include "../../sdk/definitions/cusercmd.h"
#include "../../sdk/handle_utils.h"
#include "../../sdk/helpers/helper.h"
#include "../entitylist/entitylist.h"

#include "hitscan/aimbot_hitscan.h"
#include "melee/aimbot_melee.h"
#include <algorithm>

namespace Aimbot
{
	extern AimbotState m_state;
	Vector GetAngle();
	bool IsRunning();
	bool ShouldSilent();
	void Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd);
	void DrawFOVIndicator();
	void RunPaint();
}; // namespace Aimbot