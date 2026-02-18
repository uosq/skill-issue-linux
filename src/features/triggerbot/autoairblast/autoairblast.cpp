#include "autoairblast.h"

// This is probabaly not as good as doing with traces and stuff
// But couldn't get it to work lol
bool AutoAirblast::CanAirblastHit(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CBaseEntity* pTarget, Vector& vecForward)
{
	Vector dir = (pTarget->GetCenter() - pLocal->GetCenter());

	// check distance
	if (dir.Length() > 276.0f)
		return false;

	float dot = vecForward.Dot(dir);
	float flAirblastConeScale = std::clamp(1.0f - AttributeHookValue(0.2f, "mult_airblast_cone_scale", pWeapon, nullptr, true), 0.0f, 1.0f);

	return dot >= flAirblastConeScale;
}

void LegitAirblast(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd)
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

		if (!AutoAirblast::CanAirblastHit(pLocal, pWeapon, entry.ptr, viewForward))
			continue;

		pCmd->buttons |= IN_ATTACK2;
		return;
	}
}

void RageAirblast(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, bool* pSendPacket)
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

void AutoAirblast::Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, bool* pSendPacket)
{
	if (!pWeapon->CanAirblast())
		return;

	if (Settings::Triggerbot::autoairblast == static_cast<int>(AutoBackstabMode::LEGIT))
		LegitAirblast(pLocal, pWeapon, pCmd);
	else
	 	RageAirblast(pLocal, pWeapon, pCmd, pSendPacket);
}

std::string AutoAirblast::GetModeName()
{
	switch(static_cast<AutoBackstabMode>(Settings::Triggerbot::autoairblast))
	{
        case AutoBackstabMode::NONE: return "None";
        case AutoBackstabMode::LEGIT: return "Legit";
        case AutoBackstabMode::RAGE: return "Rage";
        default: return "Invalid";
        }
}