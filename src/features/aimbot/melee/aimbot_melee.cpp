#include "aimbot_melee.h"

#include "../../prediction/prediction.h"
#include "../../backtrack/backtrack.h"

#include "../aimbot.h"

//#include "../../../sdk/classes/cobjectsentrygun.h"

static bool CanHit
(
	CTFWeaponBase* pWeapon,
	CBaseEntity* pTarget,
	const Vec3& vecShootPos,
	const Vec3& vecForward,
	float target_dist // distance to the target's current or predicted position
)
{
	assert(pWeapon && "pWeapon is null");
	assert(pTarget && "pTarget is null");

	Vec3 swingMins {}, swingMaxs {};
	float fBoundsScale = AttributeHookValue(1.0f, "melee_bounds_multiplier", pWeapon, nullptr, true);

	swingMins.Set(-18 * fBoundsScale, -18 * fBoundsScale, -18 * fBoundsScale);
	swingMaxs.Set(18 * fBoundsScale, 18 * fBoundsScale, 18 * fBoundsScale);

	float swingRange = pWeapon->GetSwingRange();

	CGameTrace trace {};
	CTraceFilterHitscan filter {};
	filter.pSkip = HandleAs<CTFPlayer*>(pWeapon->m_hOwner());

	helper::engine::TraceHull
	(
		vecShootPos, vecShootPos + (vecForward * swingRange),
		swingMins, swingMaxs,
		MASK_SHOT_HULL, &filter, &trace
	);

	if (trace.m_pEnt == pTarget)
		return true;

	float obstacle_dist = trace.fraction * swingRange;

	if (obstacle_dist >= (target_dist - (pTarget->m_vecMaxs()).Length2D()))
		return true;

	return false;
}

static bool CanWrenchHitBuilding(CBaseEntity* pTarget, CTFWeaponBase* pWeapon)
{
	assert(pTarget && "pTarget is null");
	assert(pWeapon && "pWeapon is null");

	if (!pTarget->IsBuilding() || pWeapon->GetWeaponID() != TF_WEAPON_WRENCH)
		return false;

	CBaseObject* pObj = static_cast<CBaseObject*>(pTarget);
	assert(pObj && "pObj is null");

	int max_level = pObj->m_iHighestUpgradeLevel();
	int current_level = pObj->m_iUpgradeLevel();

	int health = pObj->m_iHealth();
	int max_health = pObj->m_iMaxHealth();

	if (current_level >= max_level && health >= max_health)
		return false;

	return true;
}

static bool ShouldPredictSwing(CTFPlayer* pLocal, CTFWeaponBase* pWeapon)
{
	int local_team = pLocal->m_iTeamNum();
	float swing_range = pWeapon->GetSwingRange() * (config::aimbot::swing_melee_range.Get()/100.0f);
	bool is_wrench = pWeapon->GetWeaponID() == TF_WEAPON_WRENCH;
	bool can_hit_teammates = pWeapon->CanHitTeammates();

	Vec3 viewAngles; interfaces::Engine->GetViewAngles(viewAngles);
	Vec3 vecForward; Math::AngleVectors(viewAngles, &vecForward);
	Vec3 shootPos {};

	float smack_delay = pWeapon->GetSmackDelay() + features::backtrack.GetInterp() + features::backtrack.GetLatency();

	if (config::aimbot::swing_melee_local.Get())
	{
		std::vector<Vec3> path;

		features::prediction.BeginPrediction(pLocal, smack_delay);

		if (!features::prediction.Simulate(path) || path.empty())
		{
			features::prediction.EndPrediction();
			return false;
		}

		features::prediction.EndPrediction();

		shootPos = path.back() + pLocal->m_vecViewOffset();
	}
	else
	{
		shootPos = pLocal->GetEyePos();
	}

	for (const auto& entry : AimbotUtils::GetTargets(can_hit_teammates, local_team))
	{
		if (entry.ptr == nullptr || (entry.flags & (EntityFlags::IsAlive | EntityFlags::IsBuilding | EntityFlags::IsPlayer)) == 0)
			continue;

		bool is_teammate = entry.ptr->m_iTeamNum() == local_team;
		if (is_teammate)
		{
			if (is_wrench)
			{
				if (!(entry.flags & EntityFlags::IsBuilding) || !CanWrenchHitBuilding(entry.ptr, pWeapon))
					continue;
			}
			else if (!can_hit_teammates)
				continue;
		}

		Vec3 old_origin = entry.ptr->GetAbsOrigin();
		
		if (entry.flags & EntityFlags::IsPlayer)
		{
			std::vector<Vector> path;

			features::prediction.BeginPrediction(static_cast<CTFPlayer*>(entry.ptr), smack_delay);

			if (features::prediction.Simulate(path) && !path.empty())
				entry.ptr->SetAbsOrigin(path.back());

			features::prediction.EndPrediction();
		}

		Vec3 center = entry.ptr->GetCenter();
		Vec3 dir = center - shootPos;
		float distance = dir.Normalize();

		bool can_hit = (distance <= (swing_range * 2.5f) && CanHit(pWeapon, entry.ptr, shootPos, dir, distance));
		
		entry.ptr->SetAbsOrigin(old_origin);

		if (can_hit)
			return true; // we can hit someone
	}

	return false;
}

static inline bool LocalPlayerHasMetal(CTFPlayer* pLocal)
{
	return pLocal->m_iAmmo()[TF_AMMO_METAL] > 0;
}

static bool ApplyAim(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CBaseEntity* pTarget, AimbotState& state, const Vec3& viewAngles, Vec3 targetAngles, CUserCmd* pCmd)
{
	assert(pLocal && "pLocal is null");
	assert(pTarget && "pTarget is null");
	assert(pCmd && "pCmd is null");

	state.shouldSilent = false;
	state.running = true;

	bool shooting = false;

	switch ((AimbotMode)config::aimbot::melee_method.Get())
	{
	case AimbotMode::PLAIN:
	{
		interfaces::Engine->SetViewAngles(targetAngles);
		pCmd->viewangles = targetAngles;
		state.angle = targetAngles;

		if (config::aimbot::autoshoot.Get())
			pCmd->buttons |= IN_ATTACK;

		if (helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
			shooting = true;

		break;
	}

	case AimbotMode::SMOOTH:
	case AimbotMode::ASSISTANCE:
	{
		if (config::aimbot::melee_method.Get() == (int)AimbotMode::ASSISTANCE)
		{
			if (pCmd->mousedx == 0 && pCmd->mousedy == 0)
				break;
		}

		Vec3 smoothedAngle = AimbotUtils::GetSmoothedAngle(viewAngles, targetAngles);

		state.angle = smoothedAngle;
		pCmd->viewangles = smoothedAngle;
		interfaces::Engine->SetViewAngles(smoothedAngle);

		CBaseEntity* pAimEntity = AimbotUtils::LookingAtEntity(pLocal, smoothedAngle);

		if (pAimEntity != pTarget)
			break;

		if (config::aimbot::autoshoot.Get())
			pCmd->buttons |= IN_ATTACK;

		if (helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
			shooting = true;

		break;
	}

	case AimbotMode::SILENT:
	{
		if (config::aimbot::autoshoot.Get())
			pCmd->buttons |= IN_ATTACK;

		if (helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
		{
			state.shouldSilent = true;
			pCmd->viewangles = targetAngles;
			state.angle = targetAngles;
			shooting = true;
		}

		break;
	}

	case AimbotMode::INVALID:
	case AimbotMode::MAX:
	break;
	}

	return shooting;
}

// pretty legit ngl
static bool LegitMelee(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, AimbotState& state)
{
	assert(pLocal && "Local Player is null");
	assert(pWeapon && "Local Weapon is null");
	assert(pCmd && "UserCmd is null");

	CGameTrace trace {};

	if (!pWeapon->DoSwingTrace(trace))
		return false;

	if (!trace.DidHit() || trace.m_pEnt == nullptr)
		return false;

	CBaseEntity* pTarget = trace.m_pEnt;

	// check if its engineer's wrench
	if (pWeapon->GetWeaponID() == TF_WEAPON_WRENCH)
	{
		bool is_teammate = pTarget->m_iTeamNum() == pLocal->m_iTeamNum();

		if (is_teammate)
		{
			const bool bHasMetal = LocalPlayerHasMetal(pLocal);

			if (!bHasMetal && pTarget->IsBuilding())
				return false;

			if (!CanWrenchHitBuilding(pTarget, pWeapon))
				return false;
		}
	}

        state.angle = pCmd->viewangles;
	state.shouldSilent = false;
	state.running = true;

	if (config::aimbot::autoshoot.Get())
		pCmd->buttons |= IN_ATTACK;

	features::entities.SetAimbotTarget(pTarget);

	return helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd);
}

static void RageMelee(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, AimbotState& state)
{
	assert(pLocal && "Local Player is null");
	assert(pWeapon && "Local Weapon is null");
	assert(pCmd && "UserCmd is null");

	CBaseEntity* pTarget = nullptr;

	int local_team = pLocal->m_iTeamNum();

	float smallest_fov = FLT_MAX;
	float max_fov = (MeleeMode)config::aimbot::melee_mode.Get() == MeleeMode::LEGIT ? 90 : 180;
	float swing_range = pWeapon->GetSwingRange();

	bool is_wrench = pWeapon->GetWeaponID() == TF_WEAPON_WRENCH;
	bool can_hit_teammates = pWeapon->CanHitTeammates();

	Vec3 viewAngles; interfaces::Engine->GetViewAngles(viewAngles);
	Vec3 vecForward; Math::AngleVectors(viewAngles, &vecForward);
	Vec3 shootPos = pLocal->GetEyePos();
	Vec3 targetAngles {};

	int best_tick = -1;

	for (const auto& entry : AimbotUtils::GetTargets(can_hit_teammates, local_team))
	{
		if (entry.ptr == nullptr)
			continue;

		if ((entry.flags & (EntityFlags::IsAlive | EntityFlags::IsBuilding | EntityFlags::IsPlayer)) == 0)
			continue;

		bool is_teammate = entry.ptr->m_iTeamNum() == local_team;

		if (is_teammate)
		{
			if (is_wrench)
			{
				if (!(entry.flags & EntityFlags::IsBuilding))
					continue;

				if (!CanWrenchHitBuilding(entry.ptr, pWeapon))
					continue;
			}
			else if (!can_hit_teammates)
				continue;
		}

		bool valid_hit = false;

		if (entry.flags & EntityFlags::IsPlayer)
		{
			std::vector<LagCompRecord> records;
			if (features::backtrack.GetRecords(static_cast<CTFPlayer*>(entry.ptr), records))
			{
				for (const auto& record : records)
				{
					Vec3 old_origin = entry.ptr->GetAbsOrigin();
					Vec3 old_mins = entry.ptr->m_vecMins();
					Vec3 old_maxs = entry.ptr->m_vecMaxs();

					entry.ptr->SetAbsOrigin(record.m_vecAbsOrigin);
					entry.ptr->m_vecMins() = record.m_vecMins;
					entry.ptr->m_vecMaxs() = record.m_vecMaxs;

					Vec3 center = entry.ptr->GetCenter();
					Vec3 dir = center - shootPos;
					float distance = dir.Normalize();

					if (distance <= (swing_range * 2.5f) && CanHit(pWeapon, entry.ptr, shootPos, dir, distance))
					{
						Vec3 aimAngles;
						Math::VectorAngles(dir, aimAngles);
						float fov = Math::CalcFov(viewAngles, aimAngles);

						if (fov < smallest_fov && fov <= max_fov)
						{
							smallest_fov = fov;
							pTarget = entry.ptr;
							targetAngles = aimAngles;
							best_tick = TIME_TO_TICKS(record.m_flSimTime + features::backtrack.GetInterp());
							valid_hit = true;
						}
					}

					entry.ptr->SetAbsOrigin(old_origin);
					entry.ptr->m_vecMins() = old_mins;
					entry.ptr->m_vecMaxs() = old_maxs;
				}
			}
		}

		if (!valid_hit)
		{
			Vec3 center = entry.ptr->GetCenter();
			Vec3 dir = center - shootPos;
			float distance = dir.Normalize();

			if (distance <= (swing_range * 2.5f) && CanHit(pWeapon, entry.ptr, shootPos, dir, distance))
			{
				Vec3 aimAngles;
				Math::VectorAngles(dir, aimAngles);
				float fov = Math::CalcFov(viewAngles, aimAngles);

				if (fov < smallest_fov && fov <= max_fov)
				{
					smallest_fov = fov;
					pTarget = entry.ptr;
					targetAngles = aimAngles;
					best_tick = -1; // not backtrack
				}
			}
		}
	}

	if (pTarget == nullptr)
		return;

	if (best_tick != -1)
		pCmd->tick_count = best_tick;

	if (ApplyAim(pLocal, pWeapon, pTarget, state, viewAngles, targetAngles, pCmd))
		AimbotUtils::ShootCallback(pCmd, pTarget);

	features::entities.SetAimbotTarget(pTarget);
}

void AimbotMelee::Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, AimbotState &state)
{
	if (!config::aimbot::key.Get().IsActive())
		return;

	if (config::aimbot::melee_mode.Get() == static_cast<int>(MeleeMode::NONE) || pWeapon->GetWeaponID() == TF_WEAPON_KNIFE)
		return;

	if (config::aimbot::swing_pred.Get())
	{
		if (ShouldPredictSwing(pLocal, pWeapon))
			pCmd->buttons |= IN_ATTACK; 
	}

	MeleeMode mode = (MeleeMode)config::aimbot::melee_mode.Get();

	switch (mode)
	{
	case MeleeMode::LEGIT:
	LegitMelee(pLocal, pWeapon, pCmd, state);
	break;

	case MeleeMode::RAGE:
	RageMelee(pLocal, pWeapon, pCmd, state);
	break;

	case MeleeMode::NONE:
	default:
	break;
	}
}