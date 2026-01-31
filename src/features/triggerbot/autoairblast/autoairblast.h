#pragma once

#include "../../../sdk/classes/player.h"
#include "../../../sdk/classes/weaponbase.h"
#include "../../../sdk/definitions/cusercmd.h"
#include "../../../sdk/definitions/cgametrace.h"
#include "../../../sdk/definitions/ctracefilters.h"
#include "../../../sdk/helpers/helper.h"
#include "../../entitylist/entitylist.h"

class AutoAirblast
{
public:
	void Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd);
	bool CanAirblastHit(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CBaseEntity* pTarget, Vector& vecForward);
};

inline bool AutoAirblast::CanAirblastHit(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CBaseEntity* pTarget, Vector& vecForward)
{
	Vector dir = (pTarget->GetCenter() - pLocal->GetCenter());
	float distance = dir.Normalize();
	if (distance > pWeapon->GetDeflectionSize().Length())
		return false;

	float dot = vecForward.Dot(dir);
	float flAirblastConeScale = std::clamp(1.0f - AttributeHookValue(0.2f, "mult_airblast_cone_scale", pWeapon, nullptr, true), 0.0f, 1.0f);

	return dot > flAirblastConeScale;
}

inline void AutoAirblast::Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd)
{
	if (!pWeapon->CanAirblast())
		return;

	Vector viewAngles; interfaces::Engine->GetViewAngles(viewAngles);
	Vector viewForward; Math::AngleVectors(viewAngles, &viewForward);
	viewForward.Normalize();

	CGameTrace trace;
	CTraceFilterHitscan filter;

	float mult = AttributeHookValue(1.0f, "deflection_size_multiplier", pWeapon, nullptr, true);
	//Vector size = pWeapon->GetDeflectionSize() * mult;
	//Vector shootPos = pLocal->GetCenter();

	for (auto& entry : EntityList::GetEntities())
	{
		if (!(entry.flags & EntityFlags::IsProjectile))
			continue;

		if (!CanAirblastHit(pLocal, pWeapon, entry.ptr, viewForward))
			continue;

		pCmd->buttons |= IN_ATTACK2;
		return;
	}
}

inline AutoAirblast g_AutoAirblast;