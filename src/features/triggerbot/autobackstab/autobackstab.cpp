#include "autobackstab.h"

#include "../../backtrack/backtrack.h"

bool AutoBackstab::IsBehindEntity(Vector localCenter, Vector targetCenter, Vector targetViewAngles)
{
	Vector dir = targetCenter - localCenter;
	dir.z	   = 0;
	dir.Normalize();

	Vector targetForward;
	Math::AngleVectors(targetViewAngles, &targetForward);
	targetForward.z = 0;
	targetForward.Normalize();

	return dir.Dot(targetForward) > 0.0f;
}

bool AutoBackstab::IsBehindEntity(CTFPlayer *pLocal, CTFPlayer *pTarget)
{
	Vector vecToTarget;
	vecToTarget = pTarget->GetCenter() - pLocal->GetCenter();
	vecToTarget.z = 0.0f;
	vecToTarget.Normalize();

	// Get target forward view vector
	Vector vecTargetForward;
	Math::AngleVectors( pTarget->m_angEyeAngles(), &vecTargetForward, NULL, NULL );
	vecTargetForward.z = 0.0f;
	vecTargetForward.Normalize();

	// Make sure owner is behind, facing and aiming at target's back
	float flPosVsTargetViewDot = vecToTarget.Dot( vecTargetForward );	// Behind?

	return flPosVsTargetViewDot > 0.f;
}

bool AutoBackstab::IsBehindAndFacingEntity(CTFPlayer *pLocal, CTFPlayer *pTarget)
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
	float posVsLocalView  = dir.Dot(localForward);
	float viewAnglesDot   = localForward.Dot(targetForward);

	bool isBehind	      = posVsTargetView > 0.0f; // for some reason this is positive, but in the
							// tf2's source code it is negative wtf
	bool isLookingAtTarget = posVsLocalView > 0.5f;
	bool isFacingBack      = viewAnglesDot > -0.3f;

	return isBehind && isLookingAtTarget && isFacingBack;
}

bool AutoBackstab::IsBehindAndFacingEntity(Vector localCenter, Vector targetCenter, Vector localViewAngles,
					   Vector targetViewAngles)
{
	Vector vecToTarget;
	vecToTarget = targetCenter - localCenter;
	vecToTarget.z = 0.0f;
	vecToTarget.Normalize();

	// Get owner forward view vector
	Vector vecOwnerForward;
	Math::AngleVectors(localViewAngles, &vecOwnerForward);
	vecOwnerForward.z = 0.0f;
	vecOwnerForward.Normalize();

	// Get target forward view vector
	Vector vecTargetForward;
	Math::AngleVectors( targetViewAngles, &vecTargetForward, NULL, NULL );
	vecTargetForward.z = 0.0f;
	vecTargetForward.Normalize();

	// Make sure owner is behind, facing and aiming at target's back
	float flPosVsTargetViewDot = vecToTarget.Dot( vecTargetForward );	// Behind?
	float flPosVsOwnerViewDot = vecToTarget.Dot( vecOwnerForward );		// Facing?
	float flViewAnglesDot = vecTargetForward.Dot( vecOwnerForward );	// Facestab?

	return (flPosVsTargetViewDot > 0.f && flPosVsOwnerViewDot > 0.5 && flViewAnglesDot > -0.3);
}

bool AutoBackstab::CanBackstabEntity(CTFPlayer *pLocal, CTFPlayer *pTarget)
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

void LegitBackstab(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd)
{
	Vector localCenter     = pLocal->GetCenter();
	Vector localViewAngles = pCmd->viewangles;

	for (const auto &entry : EntityList::GetEnemies())
	{
		if ((entry.flags & (EntityFlags::IsAlive | EntityFlags::IsPlayer)) == 0)
			continue;

		CTFPlayer *pPlayer = static_cast<CTFPlayer *>(entry.ptr);
		if (pPlayer == nullptr)
			continue;

		if (!AimbotUtils::IsValidEntity(pPlayer))
			continue;

		if (AutoBackstab::IsBehindAndFacingEntity(localCenter, pPlayer->GetCenter(), localViewAngles, pPlayer->m_angEyeAngles()))
		{
			if ((pPlayer->GetCenter() - localCenter).Length() <= (48 * 2))
			{
				pCmd->buttons |= IN_ATTACK;

				if (helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
					return;
			}
		}

		std::vector<LagCompRecord> records;
		if (!Backtrack::GetRecords(pPlayer, records))
			continue;

		for (const auto &record : records)
		{
			if (!AutoBackstab::IsBehindAndFacingEntity(localCenter, record.m_vecAbsCenter, localViewAngles,
								   record.m_vecViewAngles))
				continue;

			if ((record.m_vecAbsCenter - localCenter).Length() > (48 * 2))
				continue;

			pCmd->buttons |= IN_ATTACK;

			if (helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
			{
				pCmd->tick_count = TIME_TO_TICKS(record.m_flSimTime + Backtrack::GetInterp());
				return;
			}
		}
	}
}

void RageBackstab(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, bool *pSendPacket)
{
	Vector localCenter = pLocal->GetCenter();

	for (const auto &entry : EntityList::GetEnemies())
	{
		if ((entry.flags & (EntityFlags::IsAlive | EntityFlags::IsPlayer)) == 0)
			continue;

		CTFPlayer *pPlayer = static_cast<CTFPlayer *>(entry.ptr);
		if (pPlayer == nullptr)
			continue;

		if (!AimbotUtils::IsValidEntity(pPlayer))
			continue;

		if (AutoBackstab::IsBehindEntity(localCenter, pPlayer->GetCenter(), pPlayer->m_angEyeAngles()))
		{
			if ((pPlayer->GetCenter() - localCenter).Length() <= (48 * 2))
			{
				pCmd->buttons |= IN_ATTACK;

				if (helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
				{
					Vector dir = pPlayer->GetCenter() - localCenter;
					pCmd->viewangles = dir.ToAngle();
					*pSendPacket = false;
					return;
				}
			}
		}

		std::vector<LagCompRecord> records;
		if (!Backtrack::GetRecords(pPlayer, records))
			continue;

		for (const auto &record : records)
		{
			if (!AutoBackstab::IsBehindEntity(localCenter, record.m_vecAbsCenter, record.m_vecViewAngles))
				continue;

			if ((record.m_vecAbsCenter - localCenter).Length() > (48 * 2))
				continue;

			pCmd->buttons |= IN_ATTACK;

			if (helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
			{
				Vector dir	 = record.m_vecAbsCenter - localCenter;
				pCmd->viewangles = dir.ToAngle();
				pCmd->tick_count = TIME_TO_TICKS(record.m_flSimTime + Backtrack::GetInterp());
				*pSendPacket	 = false;
				return;
			}
		}
	}
}

void AutoBackstab::Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, bool *pSendPacket)
{
	if (pLocal == nullptr || pWeapon == nullptr)
		return;

	if (pWeapon->GetWeaponID() != TF_WEAPON_KNIFE)
		return;

	switch (static_cast<GenericMode>(Config.trigger.packed.autobackstab))
	{
	case GenericMode::NONE:
		break;

	case GenericMode::LEGIT:
	{
		LegitBackstab(pLocal, pWeapon, pCmd);
		break;
	}

	case GenericMode::RAGE:
	{
		RageBackstab(pLocal, pWeapon, pCmd, pSendPacket);
		break;
	}

	default:
		break;
	}
}