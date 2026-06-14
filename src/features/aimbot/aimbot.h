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

BEGIN_CONFIG(aimbot)
	// GLOBAL
	ADD_CONFIG(fov, "aimbot fov", 0.0f)
	ADD_CONFIG(fov_indicator, "aimbot fov indicator", false)
	ADD_CONFIG(key, "aimbot key", Hotkey{})
	ADD_CONFIG(ignore_cloaked, "aimbot ignore cloaked", false)
	ADD_CONFIG(ignore_bonked, "aimbot ignore bonked", false)
	ADD_CONFIG(ignore_jarate, "aimbot ignore jarate", false)
	ADD_CONFIG(ignore_uber, "aimbot ignore uber", false)
	ADD_CONFIG(ignore_hoovy, "aimbot ignore hoovy", false)
	ADD_CONFIG(team_selected, "aimbot team", 0)
	ADD_CONFIG(smoothness, "aimbot smoothness", 0.0f)
	ADD_CONFIG(autoshoot, "aimbot autoshoot", false)
	ADD_CONFIG(wait_for_charge, "aimbot wait for charge", false)

	// METHODS
	ADD_CONFIG(hitscan_method, "aimbot hitscan method", 0)
	ADD_CONFIG(melee_method, "aimbot melee method", 0)
	ADD_CONFIG(projectile_method, "aimbot projectile method", 0)

	// HITSCAN
	ADD_CONFIG(hold_minigun_spin, "aimbot hold minigun spin", false)

	// MELEE
	ADD_CONFIG(melee_mode, "aimbot melee mode", 0)
	ADD_CONFIG(swing_melee_range, "aimbot swing pred range", 1.0f)
	ADD_CONFIG(swing_melee_local, "aimbot swing pred local", false)
	ADD_CONFIG(swing_pred, "aimbot swing pred enabled", false)

	// PROJECTILE
	ADD_CONFIG(max_sim_time, "aimbot max sim time", 0.0f)
	ADD_CONFIG(draw_predicted_player_path, "aimbot predicted player path", false)
	ADD_CONFIG(draw_predicted_player_indicator, "aimbot predicted player indicator", false)

	ADD_CONFIG(viewmodel_aim, "aimbot viewmodel aim", false)
END_CONFIG()

enum class AimbotMode
{
	INVALID = -1,
	PLAIN,
	SMOOTH,
	ASSISTANCE,
	SILENT,
	MAX
};

enum class TeamMode
{
	INVALID = -1,
	ONLYENEMY,
	ONLYTEAMMATE,
	BOTH,
	MAX
};

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