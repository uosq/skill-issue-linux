#pragma once

#include <algorithm>

#include "../../sdk/classes/player.h"
#include "../../sdk/classes/weaponbase.h"
#include "../../sdk/definitions/cusercmd.h"
#include "../../sdk/handle_utils.h"
#include "../../sdk/helpers/helper.h"

#include "../entitylist/entitylist.h"

#include "hitscan/aimbot_hitscan.h"
#include "melee/aimbot_melee.h"
#include "projectile/projectile.h"
#include "utils/utils.h"

#include "../feature.h"

class Aimbot
{
public:
	Vector GetAngle();
	bool IsRunning();
	bool ShouldSilent();
	void Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd);
	void OnImGui(ImDrawList* pDraw);
	void Reset();

	AimbotState& GetState();

private:
	AimbotState m_state {};
	AimbotMelee m_melee {};
	CAimbotProjectile m_projectile {};
	AimbotHitscan m_hitscan {};
}; // namespace Aimbot

DECLARE_FEATURE(Aimbot, aimbot)