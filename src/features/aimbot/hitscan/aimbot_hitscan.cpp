#include "aimbot_hitscan.h"

#include "../../backtrack/backtrack.h"

HitscanOffset AimbotHitscan::GetInitialOffset(CTFPlayer *pLocal, CTFWeaponBase *pWeapon)
{
	if (pWeapon == nullptr)
		return HitscanOffset::CHEST;

	switch (pWeapon->GetWeaponID())
	{
	case TF_WEAPON_REVOLVER:
	{
		if (pWeapon->CanAmbassadorHeadshot())
			return HitscanOffset::HEAD;

		return HitscanOffset::CHEST;
	}

	case TF_WEAPON_SNIPERRIFLE:
	case TF_WEAPON_SNIPERRIFLE_DECAP:
	case TF_WEAPON_SNIPERRIFLE_CLASSIC:
	{
		if (!pLocal->InCond(TF_COND_ZOOMED))
			return HitscanOffset::CHEST;

		if (static_cast<CTFSniperRifle *>(pWeapon)->m_flChargedDamage() < 50.0f)
			return HitscanOffset::CHEST;

		return HitscanOffset::HEAD;
	}
	}

	return HitscanOffset::CHEST;
}

bool AimbotHitscan::GetShotPosition(CTFPlayer *pLocal, CBaseEntity *pTarget, CTFWeaponBase *pWeapon, Vector eyePos, Vector &shotPosition, matrix3x4* pBones)
{
	matrix3x4 localBones[MAXSTUDIOBONES];

	// Fallback to current bones if no backtrack record is passed
	if (!pBones)
	{
		if (!pTarget->SetupBones(localBones, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, interfaces::GlobalVars->curtime))
			return false;
		pBones = localBones;
	}

	CGameTrace trace;
	CTraceFilterHitscan filter;
	filter.pSkip       = pLocal;

	auto initialOffset = GetInitialOffset(pLocal, pWeapon);
	switch (initialOffset)
	{
	case HitscanOffset::HEAD:
	{
		Vector boneCenter;
		static_cast<CBaseAnimating *>(pTarget)->GetHitboxCenter(pBones, HITBOX_HEAD, boneCenter);

		helper::engine::Trace(eyePos, boneCenter, MASK_SHOT | CONTENTS_HITBOX, &filter, &trace);
		if (!trace.DidHit() || trace.m_pEnt != pTarget)
			break;

		shotPosition = boneCenter;
		return true;
	}
	case HitscanOffset::CHEST:
	{
		Vector boneCenter;
		static_cast<CBaseAnimating *>(pTarget)->GetHitboxCenter(pBones, HITBOX_SPINE0, boneCenter);

		helper::engine::Trace(eyePos, boneCenter, MASK_SHOT | CONTENTS_HITBOX, &filter, &trace);
		if (!trace.DidHit() || trace.m_pEnt != pTarget)
			break;

		shotPosition = boneCenter;
		return true;
	}
	}

	for (int i = 0; i < HITBOX_LEFT_UPPERARM; i++)
	{
		Vector bonePos;
		if (!static_cast<CBaseAnimating *>(pTarget)->GetHitboxCenter(pBones, i, bonePos))
			continue;

		helper::engine::Trace(eyePos, bonePos, MASK_SHOT | CONTENTS_HITBOX, &filter, &trace);
		if (!trace.DidHit() || trace.m_pEnt != pTarget)
			continue;

		shotPosition = bonePos;
		return true;
	}

	helper::engine::Trace(eyePos, pTarget->GetCenter(), MASK_SHOT | CONTENTS_HITBOX, &filter, &trace);
	if (!trace.DidHit() || trace.m_pEnt != pTarget)
		return false;

	shotPosition = pTarget->GetCenter();
	return true;
}

static bool EvaluatePlayerTarget(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CTFPlayer *pTargetEntity, CUserCmd *pCmd, const Vector &shootPos, const Vector &viewAngles, float maxFov, bool bNoFovLimit, AimbotTarget &outTarget)
{
	bool bIsSniperRifle = pWeapon->IsSniperRifle();
	bool bIsZoomed = pLocal->InCond(TF_COND_ZOOMED);
	bool bFoundRecord = false;

	outTarget.fov = FLT_MAX;

	float flLatency = features::backtrack.GetInterp() + features::backtrack.GetLatency();

	Vector targetOrigin = pTargetEntity->GetAbsOrigin();
	Vector targetVelocity = pTargetEntity->GetVelocity();

	float predictedTime = interfaces::GlobalVars->curtime + flLatency;
	matrix3x4 predictedBones[MAXSTUDIOBONES];

	if (AimbotUtils::RebuildAnimationMatrix(pTargetEntity, targetOrigin, targetVelocity, predictedTime, predictedBones))
	{
		Vector pos;
		bool bShotFound = AimbotHitscan::GetShotPosition(pLocal, pTargetEntity, pWeapon, shootPos, pos, predictedBones);

		if (bShotFound)
		{
			float distance = (pos - shootPos).Normalize();
			if (distance < 8192.f)
			{
				Vector angle = Math::CalcAngle(shootPos, pos);
				float fov = Math::CalcFov(viewAngles, angle);

				if ((bNoFovLimit || fov <= maxFov) &&
				    (!Config.aimbot.packed.waitforcharge || !bIsZoomed || !bIsSniperRifle || AimbotUtils::CanDamageWithSniperRifle(pLocal, pTargetEntity, pWeapon)))
				{
					outTarget = {angle, pos, distance, fov, pTargetEntity, predictedTime, false};
					bFoundRecord = true;
				}
			}
		}
	}

	if (!bFoundRecord && Config.backtrack.packed.enabled)
	{
		std::vector<LagCompRecord> records;
		if (features::backtrack.GetRecords(pTargetEntity, records) && !records.empty())
		{
			for (auto& record : records)
			{
				if (!record.IsValid(pCmd))
					continue;

				Vector pos;
				if (!AimbotHitscan::GetShotPosition(pLocal, pTargetEntity, pWeapon, shootPos, pos, record.m_Bones))
					continue;

				float distance = (pos - shootPos).Normalize();
				if (distance >= 8192.f)
					continue;

				Vector angle = Math::CalcAngle(shootPos, pos);
				float fov = Math::CalcFov(viewAngles, angle);

				if (!bNoFovLimit && fov > maxFov)
					continue;

				if (Config.aimbot.packed.waitforcharge && bIsZoomed && bIsSniperRifle &&
				    !AimbotUtils::CanDamageWithSniperRifle(pLocal, pTargetEntity, pWeapon))
					continue;

				if (fov < outTarget.fov)
				{
					outTarget = {angle, pos, distance, fov, pTargetEntity, record.m_flSimTime, true};
					bFoundRecord = true;
				}
			}
		}
	}

	return bFoundRecord;
}

static bool EvaluateNonPlayerTarget(CTFPlayer *pLocal, CBaseEntity *pEntity, const Vector &shootPos, const Vector &viewAngles, float maxFov, bool bNoFovLimit, AimbotTarget &outTarget)
{
	Vector pos = pEntity->GetCenter();
	float distance = (pos - shootPos).Normalize();

	if (distance >= 8192.f)
		return false;

	Vector angle = Math::CalcAngle(shootPos, pos);
	float fov = Math::CalcFov(viewAngles, angle);

	if (!bNoFovLimit && fov > maxFov)
		return false;

	CGameTrace trace;
	CTraceFilterHitscan filter;
	filter.pSkip = pLocal;

	helper::engine::Trace(shootPos, pos, MASK_SHOT | CONTENTS_HITBOX, &filter, &trace);
	if (!trace.DidHit() || trace.m_pEnt != pEntity)
		return false;

	outTarget = {angle, pos, distance, fov, pEntity, 0.f, false};
	return true;
}

static bool FindBestTarget(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, const Vector &shootPos, const Vector &viewAngles, AimbotTarget &outBestTarget)
{
	std::vector<AimbotTarget> targets;

	int localTeam = pLocal->m_iTeamNum();
	float maxFov = AimbotUtils::GetAimbotFovScaled();
	bool bNoFovLimit = Config.aimbot.fov >= 180.0f;
	bool bCanHitTeammates = pWeapon->CanHitTeammates();

	for (EntityListEntry entry : AimbotUtils::GetTargets(bCanHitTeammates, localTeam))
	{
		CBaseEntity *entity = entry.ptr;
		AimbotTarget potentialTarget;

		if (entity->IsPlayer())
		{
			if (EvaluatePlayerTarget(pLocal, pWeapon, static_cast<CTFPlayer*>(entity), pCmd, shootPos, viewAngles, maxFov, bNoFovLimit, potentialTarget))
				targets.push_back(potentialTarget);
		}
		else
		{
			if (EvaluateNonPlayerTarget(pLocal, entity, shootPos, viewAngles, maxFov, bNoFovLimit, potentialTarget))
				targets.push_back(potentialTarget);
		}
	}

	if (targets.empty())
		return false;

	std::sort(targets.begin(), targets.end(),
		[&](const AimbotTarget& a, const AimbotTarget& b) { return a.fov < b.fov; });

	outBestTarget = targets.front();
	return true;
}

static void PlainAimbot(CUserCmd* pCmd, const AimbotTarget& target)
{
	if (Config.aimbot.packed.autoshoot)
		pCmd->buttons |= IN_ATTACK;

	Vec3 angle = target.dir;
	pCmd->viewangles = angle;
	interfaces::Engine->SetViewAngles(angle);
}

static void SmoothAssistanceAimbot(CTFPlayer* pLocal, CUserCmd* pCmd, const AimbotTarget& target, const AimbotMode& mode, const Vec3& viewAngles, const Vec3& viewForward, const Vec3& shootPos, AimbotState& state)
{
	if (mode == AimbotMode::ASSISTANCE && pCmd->mousedx == 0 && pCmd->mousedy == 0)
		return;

	Vector delta = target.dir - viewAngles;
	
	delta.x = Math::NormalizeAngle(delta.x);
	delta.y = Math::NormalizeAngle(delta.y);
	
	float smoothFactor = std::max(1.0f, Config.aimbot.smoothness);
	Vector stepAngle = delta / smoothFactor;
	Vector smoothedAngle = viewAngles + stepAngle;
	
	state.angle = smoothedAngle;
	interfaces::Engine->SetViewAngles(smoothedAngle);
	pCmd->viewangles = smoothedAngle;
	
	state.running = true;

	CGameTrace trace;
	CTraceFilterHitscan filter;
	filter.pSkip = pLocal;
	helper::engine::Trace(shootPos, shootPos + (viewForward * 2048), MASK_SHOT | CONTENTS_HITBOX, &filter, &trace);

	if (trace.DidHit() && trace.m_pEnt == target.entity)
	{
		if (Config.aimbot.packed.autoshoot)
			pCmd->buttons |= IN_ATTACK;
	}
}

static void SilentAimbot(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, const AimbotTarget& target, AimbotState& state)
{
	if (Config.aimbot.packed.autoshoot)
		pCmd->buttons |= IN_ATTACK;

	if (helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
	{
		pCmd->viewangles = target.dir;
		state.angle = target.dir;
		state.running = true;
	}
}

static void ApplyAim(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, AimbotState &state, const AimbotTarget &target, const Vector &shootPos, const Vector &viewAngles, const Vector &viewForward)
{
	if (pLocal == nullptr || pWeapon == nullptr)
		return;

	if (pCmd == nullptr)
		return;

	if (target.entity == nullptr)
		return;

	AimbotMode mode = static_cast<AimbotMode>(Config.aimbot.packed.aimmode);

	switch (mode)
	{
	case AimbotMode::PLAIN:
	{
		PlainAimbot(pCmd, target);
		break;
	}
	case AimbotMode::ASSISTANCE:
	case AimbotMode::SMOOTH:
	{
		SmoothAssistanceAimbot(pLocal, pCmd, target, mode, viewAngles, viewForward, shootPos, state);
		break;
	}
	case AimbotMode::SILENT:
	{
		SilentAimbot(pLocal, pWeapon, pCmd, target, state);
		break;
	}
	case AimbotMode::INVALID:
	case AimbotMode::MAX:
		break;
	}

	features::entities.SetAimbotTarget(target.entity);

	if (target.useBacktrack && helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
		pCmd->tick_count = TIME_TO_TICKS(target.simTime + features::backtrack.GetInterp());
}

static bool IsLocalPlayerInvalid(CTFPlayer* pLocal)
{
	return pLocal == nullptr || !pLocal->IsAlive() || pLocal->IsTaunting() || pLocal->IsGhost();
}

static bool IsWeaponInvalid(CTFWeaponBase* pWeapon)
{
	return pWeapon == nullptr || !pWeapon->IsHitscan();
}

void AimbotHitscan::Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, AimbotState &state)
{
	if (IsLocalPlayerInvalid(pLocal) || IsWeaponInvalid(pWeapon))
		return;

	if (!Config.aimbot.key->IsActive())
		return;

	if (Config.aimbot.packed.waitforcharge && pWeapon->IsAmbassador())
		if (!pWeapon->CanAmbassadorHeadshot())
			return;

	if (Config.aimbot.packed.hold_minigun_spin && pWeapon->GetWeaponID() == TF_WEAPON_MINIGUN)
		pCmd->buttons |= IN_ATTACK2;

	Vector shootPos = pLocal->GetEyePos();
	Vector viewAngles;
	interfaces::Engine->GetViewAngles(viewAngles);

	Vector viewForward;
	Math::AngleVectors(viewAngles, &viewForward);
	viewForward.Normalize();

	AimbotTarget bestTarget;

	if (!FindBestTarget(pLocal, pWeapon, pCmd, shootPos, viewAngles, bestTarget))
		return;

	ApplyAim(pLocal, pWeapon, pCmd, state, bestTarget, shootPos, viewAngles, viewForward);
}