#pragma once

#include "../../../sdk/definitions/cusercmd.h"
#include "../../../sdk/classes/player.h"
#include "../../../sdk/classes/weaponbase.h"
#include "../../../sdk/helpers/helper.h"
#include "../../../sdk/handle_utils.h"
#include "../../../sdk/definitions/ienginetrace.h"
#include "../../../sdk/definitions/cgametrace.h"
#include "../../../sdk/definitions/ctracefilters.h"
#include "../../../sdk/definitions/bspflags.h"
#include "../../../settings.h"
#include "../../../sdk/definitions/studio.h"

#include "../utils/utils.h"
#include "../../entitylist/entitylist.h"

#include <cmath>
#include <vector>

enum class HitscanOffset
{
	HEAD = 0,
	CHEST,
	//PELVIS
	// legs are unreliable
};

struct AimbotHitscan
{
	HitscanOffset GetInitialOffset(CTFPlayer* pLocal, CTFWeaponBase* pWeapon)
	{
		if (pWeapon == nullptr)
			return HitscanOffset::CHEST;

		switch(pWeapon->GetWeaponID())
		{
			case TF_WEAPON_REVOLVER:
			{
				if (pWeapon->CanAmbassadorHeadshot())
					return HitscanOffset::HEAD;
			}

			case TF_WEAPON_SNIPERRIFLE:
			case TF_WEAPON_SNIPERRIFLE_DECAP:
			case TF_WEAPON_SNIPERRIFLE_CLASSIC:
			{
				if (!pLocal->InCond(TF_COND_ZOOMED))
					break;

				if (static_cast<CTFSniperRifle*>(pWeapon)->GetChargedDamage() < 150.0f)
					break;

				return HitscanOffset::HEAD;
			}

		}
		
		return HitscanOffset::CHEST;
	}

	bool GetShotPosition(CTFPlayer* pLocal, CBaseEntity* pTarget, CTFWeaponBase* pWeapon, Vector eyePos, Vector& shotPosition)
	{
		matrix3x4 bones[MAXSTUDIOBONES];
		if (!pTarget->SetupBones(bones, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, interfaces::GlobalVars->curtime))
			return false;

		CGameTrace trace;
		CTraceFilterHitscan filter;
		filter.pSkip = pLocal;

		auto initialOffset = GetInitialOffset(pLocal, pWeapon);
		switch(initialOffset)
		{
			case HitscanOffset::HEAD:
			{
				Vector boneCenter;
				static_cast<CBaseAnimating*>(pTarget)->GetHitboxCenter(bones, HITBOX_HEAD, boneCenter);

				helper::engine::Trace(eyePos, boneCenter, MASK_SHOT | CONTENTS_HITBOX, &filter, &trace);
				if (!trace.DidHit() || trace.m_pEnt != pTarget)
					break;

				shotPosition = boneCenter;
				return true;
			}
			case HitscanOffset::CHEST:
			{
				Vector boneCenter;
				static_cast<CBaseAnimating*>(pTarget)->GetHitboxCenter(bones, HITBOX_SPINE3, boneCenter);

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
			if (!static_cast<CBaseAnimating*>(pTarget)->GetHitboxCenter(bones, i, bonePos))
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

	void Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, AimbotState& state)
	{
		std::vector<PotentialTarget> targets;

		int localTeam = pLocal->m_iTeamNum();
		Vector shootPos = pLocal->GetEyePos();

		Vector viewAngles;
		interfaces::Engine->GetViewAngles(viewAngles);

		Vector viewForward;
		Math::AngleVectors(viewAngles, &viewForward);
		viewForward.Normalize();

		float fovRad = DEG2RAD(settings.aimbot.fov);
		float minDot = cosf(fovRad);

		CGameTrace trace;
		CTraceFilterHitscan filter;
		filter.pSkip = pLocal;

		//HitscanOffset initialOffset = GetInitialOffset(pWeapon);

		for (const auto& entity : EntityList::GetEnemies())
		{
			Vector pos;
			{
				if (entity->IsPlayer())
					GetShotPosition(pLocal, entity, pWeapon, shootPos, pos);
				else if (entity->IsBuilding())
					pos = reinterpret_cast<CBaseObject*>(entity)->GetCenter();
				else
					pos = entity->GetAbsOrigin();
			}

			Vector dir = pos - shootPos;
			float distance = dir.Normalize();

			if (distance >= 2048.f)
				continue;

			float dot = dir.Dot(viewForward);

			if (dot < minDot)
				continue;

			/*helper::engine::Trace(shootPos, pos, MASK_SHOT | CONTENTS_HITBOX, &filter, &trace);
			if (!trace.DidHit() || trace.m_pEnt != entity)
				continue;*/

			targets.emplace_back(PotentialTarget{dir, pos, distance, dot, entity});
		}

		if (targets.empty())
			return;

		AimbotUtils::QuickSort(targets, 0, targets.size() - 1);

		if (settings.aimbot.autoshoot)
			pCmd->buttons |= IN_ATTACK;

		if (helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
		{
			auto target = targets.front();
			Vector angle = target.dir.ToAngle();
			pCmd->viewangles = angle;
			state.angle = angle;
			state.running = true;
		}

		if (targets.front().entity != nullptr)
			EntityList::m_pAimbotTarget = targets.front().entity;
	}
};