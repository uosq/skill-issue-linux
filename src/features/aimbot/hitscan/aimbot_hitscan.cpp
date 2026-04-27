#include "aimbot_hitscan.h"

#include "../../backtrack/backtrack.h"

struct AimbotTarget 
{
	Vector dir;
	Vector pos;
	float distance;
	float fov;
	CBaseEntity* entity;
	float simTime;
	bool useBacktrack;
};

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

void AimbotHitscan::Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, AimbotState &state)
{
	if (!Settings::Aimbot.key->IsActive())
		return;

	if (Settings::Aimbot.waitforcharge && pWeapon->IsAmbassador())
		if (!pWeapon->CanAmbassadorHeadshot())
			return;

	if (Settings::Aimbot.hold_minigun_spin && pWeapon->GetWeaponID() == TF_WEAPON_MINIGUN)
		pCmd->buttons |= IN_ATTACK2;

	int localTeam   = pLocal->m_iTeamNum();
	Vector shootPos = pLocal->GetEyePos();

	Vector viewAngles;
	interfaces::Engine->GetViewAngles(viewAngles);

	Vector viewForward;
	Math::AngleVectors(viewAngles, &viewForward);
	viewForward.Normalize();

	CGameTrace trace;
	CTraceFilterHitscan filter;
	filter.pSkip          = pLocal;

	bool bIsSniperRifle   = pWeapon->IsSniperRifle();
	bool bIsZoomed        = pLocal->InCond(TF_COND_ZOOMED);

	float maxFov          = AimbotUtils::GetAimbotFovScaled();
	bool bNoFovLimit      = Settings::Aimbot.fov >= 180.0f;

	bool bCanHitTeammates = pWeapon->CanHitTeammates();

	std::vector<AimbotTarget> targets;

	for (EntityListEntry entry : AimbotUtils::GetTargets(bCanHitTeammates, localTeam))
	{
		CBaseEntity *entity = entry.ptr;

		if (entity->IsPlayer())
		{
			CTFPlayer* pPlayer = static_cast<CTFPlayer*>(entity);
			std::vector<LagCompRecord> records;
			
			bool bFoundRecord = false;
			AimbotTarget bestRecordTarget;
			bestRecordTarget.fov = FLT_MAX;

			if (Backtrack::GetRecords(pPlayer, records) && !records.empty())
			{
				for (auto& record : records)
				{
					if (!record.IsValid(pCmd))
						continue;

					Vector pos;
					if (!GetShotPosition(pLocal, entity, pWeapon, shootPos, pos, record.m_Bones))
						continue;

					float distance = (pos - shootPos).Normalize();
					if (distance >= 8192.f)
						continue;

					Vector angle	= Math::CalcAngle(shootPos, pos);
					float fov	= Math::CalcFov(viewAngles, angle);

					if (!bNoFovLimit && fov > maxFov)
						continue;

					if (Settings::Aimbot.waitforcharge && bIsZoomed && bIsSniperRifle &&
					    !AimbotUtils::CanDamageWithSniperRifle(pLocal, entity, pWeapon))
						continue;

					if (fov < bestRecordTarget.fov)
					{
						bestRecordTarget = {angle, pos, distance, fov, entity, record.m_flSimTime, true};
						bFoundRecord = true;
					}
				}
			}

			if (bFoundRecord)
			{
				targets.push_back(bestRecordTarget);
				continue; // move to next entity, we already have their best record
			}

			// if no backtrack records were valid/found, check current position
			Vector pos;
			if (!GetShotPosition(pLocal, entity, pWeapon, shootPos, pos, nullptr))
				continue;

			float distance = (pos - shootPos).Normalize();
			if (distance >= 8192.f)
				continue;

			Vector angle = Math::CalcAngle(shootPos, pos);
			float fov    = Math::CalcFov(viewAngles, angle);

			if (!bNoFovLimit && fov > maxFov)
				continue;

			if (Settings::Aimbot.waitforcharge && bIsZoomed && bIsSniperRifle &&
			    !AimbotUtils::CanDamageWithSniperRifle(pLocal, entity, pWeapon))
				continue;

			targets.push_back({angle, pos, distance, fov, entity, 0.f, false});
		}
		else
		{
			// dont try to backtrack NPCs!
			Vector pos = entity->GetCenter();
			float distance = (pos - shootPos).Normalize();
			if (distance >= 8192.f)
				continue;

			Vector angle = Math::CalcAngle(shootPos, pos);
			float fov    = Math::CalcFov(viewAngles, angle);

			if (!bNoFovLimit && fov > maxFov)
				continue;

			helper::engine::Trace(shootPos, pos, MASK_SHOT | CONTENTS_HITBOX, &filter, &trace);
			if (!trace.DidHit() || trace.m_pEnt != entity)
				continue;

			targets.push_back({angle, pos, distance, fov, entity, 0.f, false});
		}
	}

	if (targets.empty())
		return;

	std::sort(targets.begin(), targets.end(),
		  [&](const AimbotTarget& a, const AimbotTarget& b) { return a.fov < b.fov; });

	AimbotTarget ptTarget = targets.front();
	AimbotMode mode = static_cast<AimbotMode>(Settings::Aimbot.mode);

	switch (mode)
	{
	case AimbotMode::PLAIN:
	{
		if (Settings::Aimbot.autoshoot)
			pCmd->buttons |= IN_ATTACK;

		Vector angle     = ptTarget.dir;

		pCmd->viewangles = angle;
		interfaces::Engine->SetViewAngles(angle);
		break;
	}
	case AimbotMode::ASSISTANCE:
	case AimbotMode::SMOOTH:
	{
		if (mode == AimbotMode::ASSISTANCE)
			if (pCmd->mousedx == 0 && pCmd->mousedy == 0)
				break;

		Vector targetAngle = ptTarget.dir;
		Vector delta = targetAngle - viewAngles;

		// fix the 180 flick
		delta.x = Math::NormalizeAngle(delta.x);
		delta.y = Math::NormalizeAngle(delta.y);
		
		float flDistance = delta.Length();

		// max allowed to move per tick
		float flMaxStep = Settings::Aimbot.smoothness;
		float flStepSize = std::min(flDistance, flMaxStep);

		Vector stepAngle = delta * (flStepSize / flDistance);
		Vector smoothedAngle = viewAngles + stepAngle;

		state.angle = smoothedAngle;
		interfaces::Engine->SetViewAngles(smoothedAngle);
		pCmd->viewangles = smoothedAngle;

		state.running = true;

		CGameTrace trace;
		CTraceFilterHitscan filter;
		filter.pSkip = pLocal;
		helper::engine::Trace(shootPos, shootPos + (viewForward * 2048), MASK_SHOT | CONTENTS_HITBOX, &filter, &trace);

		if (!trace.DidHit() || trace.m_pEnt != ptTarget.entity)
			break;

		if (Settings::Aimbot.autoshoot)
			pCmd->buttons |= IN_ATTACK;

		break;
	}
	case AimbotMode::SILENT:
	{
		if (Settings::Aimbot.autoshoot)
			pCmd->buttons |= IN_ATTACK;

		if (helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
		{
			Vector angle = ptTarget.dir;
			pCmd->viewangles = angle;
			state.angle = angle;
			state.running = true;
		}

		break;
	}

	case AimbotMode::INVALID:
	case AimbotMode::MAX:
		break;
	}

	if (targets.front().entity != nullptr)
	{
		EntityList::m_pAimbotTarget = targets.front().entity;

		if (ptTarget.useBacktrack && helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
			pCmd->tick_count = TIME_TO_TICKS(ptTarget.simTime + Backtrack::GetInterp());
	}
}