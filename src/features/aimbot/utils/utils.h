#pragma once

#include "../../../sdk/classes/cbaseobject.h"
#include "../../../sdk/classes/player.h"
#include "../../../sdk/definitions/cgametrace.h"
#include "../../../sdk/definitions/ctracefilters.h"
#include "../../../sdk/helpers/helper.h"
#include "../../../settings/settings.h"
#include "../../entitylist/entitylist.h"

struct PotentialTarget
{
	Vector dir;
	Vector center;
	float distance;
	float fov;
	CBaseEntity *entity;
};

struct AimbotState
{
	bool running	  = false;
	bool shouldSilent = false;
	Vector angle = {0, 0, 0};
};

inline void ClearAimbotState(AimbotState &state)
{
	state.shouldSilent = false;
	state.running	   = false;
}

namespace AimbotUtils
{
	bool IsValidEntity(CBaseEntity *entity);

	// Is this optimized? absolutely fucking not
	// I need to think of a better way
	// I should probably check bones
	bool GetVisiblePoint(Vector &out, CTFPlayer *pLocal, Vector origin, Vector mins, Vector maxs);

	bool CanDamageWithSniperRifle(CTFPlayer *pLocal, CBaseEntity *pTarget, CTFWeaponBase *pWeapon);
	float GetFovScaled(float flFov);
	float GetAimbotFovScaled();

	std::vector<EntityListEntry> GetTargets(const bool &bCanHitTeammates, int localTeam);

	bool RebuildAnimationMatrix(CTFPlayer* pPlayer, const Vector& predictedOrigin, const Vector& predictedVelocity, float predictedTime, matrix3x4* outBones);

	Vec3 GetSmoothedAngle(const Vec3& viewAngles, const Vec3& targetDir);
	CBaseEntity* LookingAtEntity(CTFPlayer* pLocal, const Vec3& viewAngles);

	void ShootCallback(CUserCmd* pCmd, CBaseEntity* pTarget = nullptr);
}; // namespace AimbotUtils