#pragma once

#include "../../../sdk/classes/player.h"
#include "../../../sdk/classes/cbaseobject.h"
#include "../../../settings/settings.h"
#include "../../../sdk/definitions/cgametrace.h"
#include "../../../sdk/definitions/ctracefilters.h"
#include "../../../sdk/helpers/helper.h"
#include "../../entitylist/entitylist.h"
#include "../../visuals/customfov/customfov.h"

struct PotentialTarget
{
	Vector dir;
	Vector center;
	float distance;
	float fov;
	CBaseEntity* entity;
};

struct AimbotState
{
	bool running = false;
	bool shouldSilent = false;
	//std::vector<Vector> targetPath;
	Vector angle = {0, 0, 0};
};

static void ClearAimbotState(AimbotState& state)
{
	//state.angle.Set(0, 0, 0);
	state.shouldSilent = false;
	state.running = false;
	//state.targetPath.clear();
}

namespace AimbotUtils
{
	bool IsValidEntity(CBaseEntity* entity);

	// Is this optimized? absolutely fucking not
	// I need to think of a better way
	// I should probably check bones
	bool GetVisiblePoint(Vector& out, CTFPlayer* pLocal, Vector origin, Vector mins, Vector maxs);

	std::string GetAimbotModeName();
	std::string GetTeamModeName();

	bool CanDamageWithSniperRifle(CTFPlayer* pLocal, CBaseEntity* pTarget, CTFWeaponBase* pWeapon);
	float GetAimbotFovScaled();

	std::vector<EntityListEntry> GetTargets(const bool& bCanHitTeammates, int localTeam);
};