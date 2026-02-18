#pragma once

#include "../utils/utils.h"
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
#include "../../prediction/prediction.h"
#include "../../entitylist/entitylist.h"
#include <cmath>
#include <vector>

struct ProjectileInfo_t
{
	float speed = 0;
	float gravity = 0;
	float primetime = 0;
	float damage_radius = 0;
	float lifetime = 60.0f;
	bool simple_trace = false;
	Vector offset{};
	Vector hull{6, 6, 6};
};

namespace AimbotProjectile
{
	bool GetProjectileInfo(ProjectileInfo_t& info, CTFPlayer* owner, CTFWeaponBase* pWeapon);
	// Offset before any multipointing is applied
	float GetInitialOffset(CTFPlayer* target, CTFWeaponBase* pWeapon);
	bool SolveBallisticArc(Vector &outAngle, Vector p0, Vector p1, float speed, float gravity);
	bool CheckTrajectory(CBaseEntity* target, Vector startPos, Vector targetPos, Vector angle, ProjectileInfo_t info, float gravity);
	void Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, AimbotState& state);
};