#include "autobackstab.h"

#include "../../backtrack/backtrack.h"
#include "../../logs/logs.h"

bool AutoBackstab::IsBehindEntity(Vector localCenter, Vector targetCenter, Vector targetViewAngles)
{
	Vector dir = targetCenter - localCenter;
	dir.z = 0;
	dir.Normalize();

	Vector targetForward;
	Math::AngleVectors(targetViewAngles, &targetForward);
	targetForward.z = 0;
	targetForward.Normalize();

	return dir.Dot(targetForward) > 0.0f;
}

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

	/*for (auto& entry : EntityList::GetEnemies())
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
				Vector angle = dir.ToAngle();
				pCmd->viewangles = angle;
				*pSendPacket = false;
			}

			EntityList::m_pAimbotTarget = enemy;
			break;
		}
	}*/

	Vector localCenter = pLocal->GetCenter();

	for (const auto& entry : EntityList::GetEnemies())
	{
		if ((entry.flags & (EntityFlags::IsAlive | EntityFlags::IsPlayer)) == 0)
			continue;

		CTFPlayer* pPlayer = static_cast<CTFPlayer*>(entry.ptr);
		if (pPlayer == nullptr)
			continue;

		if (!AimbotUtils::IsValidEntity(pPlayer))
			continue;

		std::vector<LagCompRecord> records;
		if (!Backtrack::GetRecords(pPlayer, records))
		{
			Logs::Info("Invalid records");
			continue;
		}

		for (const auto& record : records)
		{
			if (!AutoBackstab::IsBehindEntity(localCenter, record.absCenter, record.viewAngle))
				continue;

			if ((record.absCenter - localCenter).Length() > (48*2))
				continue;

			pCmd->buttons |= IN_ATTACK;

			if (helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
			{
				Vector dir = record.absCenter - localCenter;
				pCmd->viewangles = dir.ToAngle();
				pCmd->tick_count = TIME_TO_TICKS(record.simtime);
				*pSendPacket = false;
				return;
			}
		}
	}
}

void AutoBackstab::Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, bool* pSendPacket)
{
	if (pLocal == nullptr || pWeapon == nullptr)
		return;

	if (pWeapon->GetWeaponID() != TF_WEAPON_KNIFE)
		return;

	switch(static_cast<AutoBackstabMode>(Settings::Trigger.autobackstab))
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
	switch(static_cast<AutoBackstabMode>(Settings::Trigger.autobackstab))
	{
		case AutoBackstabMode::NONE: return "None";
		case AutoBackstabMode::LEGIT: return "Legit";
		case AutoBackstabMode::RAGE: return "Rage";
		default: return "Unknown";
        }
}