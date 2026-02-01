#pragma once

#include "../../../sdk/classes/player.h"
#include "../../../sdk/classes/weaponbase.h"
#include "../../../sdk/definitions/cusercmd.h"
#include "../../../sdk/definitions/cgametrace.h"
#include "../../../sdk/definitions/ctracefilters.h"
#include "../../../sdk/helpers/helper.h"
#include "../../entitylist/entitylist.h"
#include "../../../settings.h"
#include <bits/types/locale_t.h>

class AutoAirblast
{
private:
	void Legit(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd);
	void Rage(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, bool* pSendPacket);
public:
	void Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, bool* pSendPacket);
	bool CanAirblastHit(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CBaseEntity* pTarget, Vector& vecForward);
	std::string GetModeName();
};

// This is probabaly not as good as doing with traces and stuff
// But couldn't get it to work lol
inline bool AutoAirblast::CanAirblastHit(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CBaseEntity* pTarget, Vector& vecForward)
{
	Vector dir = (pTarget->GetCenter() - pLocal->GetCenter());

	// check distance
	if (dir.Length() > 276.0f)
		return false;

	float dot = vecForward.Dot(dir);
	float flAirblastConeScale = std::clamp(1.0f - AttributeHookValue(0.2f, "mult_airblast_cone_scale", pWeapon, nullptr, true), 0.0f, 1.0f);

	return dot >= flAirblastConeScale;
}

inline void AutoAirblast::Legit(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd)
{
	Vector viewAngles; interfaces::Engine->GetViewAngles(viewAngles);
	Vector viewForward; Math::AngleVectors(viewAngles, &viewForward);
	viewForward.Normalize();

	for (auto& entry : EntityList::GetEntities())
	{
		if (!(entry.flags & EntityFlags::IsProjectile))
			continue;

		if (!(entry.flags & EntityFlags::IsEnemy))
			continue;

		if (entry.ptr == nullptr)
			continue;

		if (!CanAirblastHit(pLocal, pWeapon, entry.ptr, viewForward))
			continue;

		pCmd->buttons |= IN_ATTACK2;
		return;
	}
}

inline void AutoAirblast::Rage(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, bool* pSendPacket)
{
	Vector viewAngles; interfaces::Engine->GetViewAngles(viewAngles);
	Vector viewForward; Math::AngleVectors(viewAngles, &viewForward);
	viewForward.Normalize();

	Vector shootPos = pLocal->GetCenter();

	for (auto& entry : EntityList::GetEntities())
	{
		if (!(entry.flags & EntityFlags::IsProjectile))
			continue;

		if (!(entry.flags & EntityFlags::IsEnemy))
			continue;

		if (entry.ptr == nullptr)
			continue;

		Vector center = entry.ptr->GetCenter();
		Vector dir = center - shootPos;
		float distance = dir.Normalize();
		if (distance > 276.0f)
			continue;

		pCmd->viewangles = dir.ToAngle();
		pCmd->buttons |= IN_ATTACK2;
		return;
	}
}

inline void AutoAirblast::Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, bool* pSendPacket)
{
	if (!pWeapon->CanAirblast())
		return;

	if (g_Settings.triggerbot.autoairblast == AutoAirblastMode::LEGIT)
		Legit(pLocal, pWeapon, pCmd);
	else
	 	Rage(pLocal, pWeapon, pCmd, pSendPacket);
}

inline std::string AutoAirblast::GetModeName()
{
	switch(g_Settings.triggerbot.autoairblast)
	{
        case AutoAirblastMode::NONE: return "None";
        case AutoAirblastMode::LEGIT: return "Legit";
        case AutoAirblastMode::RAGE: return "Rage";
        default: return "Invalid";
        }
}

inline AutoAirblast g_AutoAirblast;