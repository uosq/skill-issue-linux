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
	HitscanOffset GetInitialOffset(CTFWeaponBase* pWeapon)
	{
		if (pWeapon == nullptr)
			return HitscanOffset::CHEST;

		switch(pWeapon->GetWeaponID())
		{
			case TF_WEAPON_SNIPERRIFLE:
			case TF_WEAPON_SNIPERRIFLE_CLASSIC:
			case TF_WEAPON_SNIPERRIFLE_DECAP:
			{
				if (static_cast<CTFSniperRifle*>(pWeapon)->m_flChargedDamage() > 150)
					return HitscanOffset::HEAD;
			}
		}
		
		return HitscanOffset::CHEST;
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

		HitscanOffset initialOffset = GetInitialOffset(pWeapon);

		for (auto entity : EntityList::m_vecPlayers)
		{
			if (!AimbotUtils::IsValidEntity(entity, localTeam))
				continue;

			Vector pos = entity->GetCenter();
			Vector dir = pos - shootPos;
			float distance = dir.Normalize();

			if (distance >= 2048.f)
				continue;

			float dot = dir.Dot(viewForward);

			if (dot < minDot)
				continue;

			helper::engine::Trace(shootPos, pos, MASK_SHOT | CONTENTS_HITBOX, &filter, &trace);
			if (!trace.DidHit() || !trace.m_pEnt || trace.m_pEnt != entity)
				continue;

			targets.emplace_back(PotentialTarget{dir, pos, distance, dot, entity});
		}

		for (auto entity : EntityList::m_vecBuildings)
		{
			CBaseEntity* building = reinterpret_cast<CBaseEntity*>(entity);

			if (!AimbotUtils::IsValidEntity(building, localTeam))
				continue;

			Vector pos = entity->GetCenter();
			Vector dir = pos - shootPos;
			float distance = dir.Normalize();

			if (distance >= 2048.f)
				continue;

			float dot = dir.Dot(viewForward);

			if (dot < minDot)
				continue;

			helper::engine::Trace(shootPos, pos, MASK_SHOT | CONTENTS_HITBOX, &filter, &trace);
			if (!trace.DidHit() || !trace.m_pEnt || trace.m_pEnt != building)
				continue;

			targets.emplace_back(PotentialTarget{dir, pos, distance, dot, building});
		}

		if (targets.empty())
			return;

		AimbotUtils::QuickSort(targets, 0, targets.size() - 1);

		if (settings.aimbot.autoshoot)
			pCmd->buttons |= IN_ATTACK;

		if (pWeapon->CanPrimaryAttack() && (pCmd->buttons & IN_ATTACK))
		{
			auto target = targets.at(0);
			Vector angle = target.dir.ToAngle();
			pCmd->viewangles = angle;
			state.angle = angle;
			state.running = true;
		}
	}
};