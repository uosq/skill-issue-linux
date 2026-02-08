#include "autobackstab.h"

bool AutoBackstab::IsBehindEntity(CTFPlayer* pLocal, CTFPlayer* pTarget)
{
	Vector dir = (pTarget->GetCenter() - pLocal->GetCenter());
	dir.z = 0;
	dir.Normalize();

	Vector targetForward;
	Math::AngleVectors(pTarget->m_angEyeAngles(), &targetForward);
	targetForward.z = 0;
	targetForward.Normalize();

	float posVsTargetView = dir.Dot(targetForward);
	return posVsTargetView > 0.0f; // for some reason this is positive, but in the tf2's source code it is negative wtf
}

bool AutoBackstab::IsBehindAndFacingEntity(CTFPlayer *pLocal, CTFPlayer* pTarget)
{
	Vector dir = (pTarget->GetCenter() - pLocal->GetCenter());
	dir.z = 0;
	dir.Normalize();

	Vector localForward;
	Math::AngleVectors(pLocal->m_angEyeAngles(), &localForward);
	localForward.z = 0;
	localForward.Normalize();

	Vector targetForward;
	Math::AngleVectors(pTarget->m_angEyeAngles(), &targetForward);
	targetForward.z = 0;
	targetForward.Normalize();

	float posVsTargetView = dir.Dot(targetForward);
	float posVsLocalView = dir.Dot(localForward);
	float viewAnglesDot = localForward.Dot(targetForward);

	bool isBehind = posVsTargetView > 0.0f; // for some reason this is positive, but in the tf2's source code it is negative wtf
	bool isLookingAtTarget = posVsLocalView > 0.5f;
	bool isFacingBack = viewAnglesDot > -0.3f;

	return isBehind && isLookingAtTarget && isFacingBack;
}

bool AutoBackstab::CanBackstabEntity(CTFPlayer* pLocal, CTFPlayer* pTarget)
{
	if (pLocal == nullptr || pTarget == nullptr)
		return false;

	int iNoBackstab = AttributeHookValue(1.0, "cannot_be_backstabbed", pTarget, nullptr, true);
	if (iNoBackstab == 0)
		return false;

	if (IsBehindAndFacingEntity(pLocal, pTarget))
		return true;

	return false;
}

void LegitBackstab(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd)
{
	CGameTrace trace;
	CTraceFilterHitscan filter;
	filter.pSkip = pLocal;

	Vector viewAngles, forward;
	interfaces::Engine->GetViewAngles(viewAngles);
	Math::AngleVectors(viewAngles, &forward);

	Vector start = pLocal->GetAbsOrigin() + pLocal->m_vecViewOffset();
	Vector end = start + (forward * 48);

	int localTeam = pLocal->m_iTeamNum();

	helper::engine::Trace(start, end, MASK_SHOT_HULL, &filter, &trace);

	if (!trace.DidHit() || trace.m_pEnt == nullptr)
		return;

	if (!AimbotUtils::IsValidEntity(trace.m_pEnt))
		return;

	if (trace.m_pEnt->m_iTeamNum() == localTeam)
		return;

	if (!AutoBackstab::CanBackstabEntity(pLocal, static_cast<CTFPlayer*>(trace.m_pEnt)))
		return;

	pCmd->buttons |= IN_ATTACK;
	EntityList::m_pAimbotTarget = trace.m_pEnt;
}

void RageBackstab(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, bool* pSendPacket)
{
	Vector shootPos = pLocal->GetEyePos();

	for (auto& entry : EntityList::GetEnemies())
	{
		if (!(entry.flags & EntityFlags::IsPlayer))
			continue;

		CTFPlayer* enemy = static_cast<CTFPlayer*>(entry.ptr);
		if (!AimbotUtils::IsValidEntity(enemy))
			continue;

		Vector center = enemy->GetCenter();
		Vector dir = center - shootPos;
		float distance = dir.Normalize();

		if (distance > (48*2))
			continue;

		if (AutoBackstab::IsBehindEntity(pLocal, enemy))
		{
			pCmd->buttons |= IN_ATTACK;

			if (helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
			{
				Vector viewAngles, viewForward;
				interfaces::Engine->GetViewAngles(viewAngles);
				Math::AngleVectors(viewAngles, &viewForward);
					
				Vector angle = dir.ToAngle();
				pCmd->viewangles = angle;
				*pSendPacket = false;
			}

			EntityList::m_pAimbotTarget = enemy;
			break;
		}
	}
}

void AutoBackstab::Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, bool* pSendPacket)
{
	if (pLocal == nullptr || pWeapon == nullptr)
		return;

	if (pWeapon->GetWeaponID() != TF_WEAPON_KNIFE)
		return;

	switch(Settings::triggerbot.autobackstab)
	{
		case AutoBackstabMode::NONE:
		break;

		case AutoBackstabMode::LEGIT:
		{
			LegitBackstab(pLocal, pWeapon, pCmd);
			break;
		}

		case AutoBackstabMode::RAGE:
		{
			RageBackstab(pLocal, pWeapon, pCmd, pSendPacket);
			break;
		}

		default:
		break;
        }
}

std::string AutoBackstab::GetModeName()
{
	switch(Settings::triggerbot.autobackstab)
	{
		case AutoBackstabMode::NONE: return "None";
		case AutoBackstabMode::LEGIT: return "Legit";
		case AutoBackstabMode::RAGE: return "Rage";
		default: return "Unknown";
        }
}