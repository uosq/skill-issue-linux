#include "aimbot_melee.h"

namespace AimbotMelee
{
	void Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, AimbotState& state)
	{
		if (Settings::Aimbot::melee == static_cast<int>(MeleeMode::NONE) || pWeapon->GetWeaponID() == TF_WEAPON_KNIFE)
			return;
		
		std::vector<PotentialTarget> targets;

		bool is_a_sword = static_cast<int>(AttributeHookValue(0, "is_a_sword", pWeapon, nullptr, true));
		float range = (is_a_sword ? 72.0f : 48.0f) * 1.9f;
		int localTeam = pLocal->m_iTeamNum();

		Vector shootPos = pLocal->GetEyePos();
		
		CGameTrace trace;
		CTraceFilterHitscan filter;
		filter.pSkip = pLocal;
		
		Vector swingMins, swingMaxs;
		float fBoundsScale = AttributeHookValue(1.0f, "melee_bounds_multiplier", pWeapon, nullptr, true);
		swingMins.Set(-18 * fBoundsScale, -18 * fBoundsScale, -18 * fBoundsScale);
		swingMaxs.Set(18 * fBoundsScale, 18 * fBoundsScale, 18 * fBoundsScale);

		Vector targetAngle;
		CBaseEntity* target = nullptr;
		float highestDot = -1.0f;

		MeleeMode mode = static_cast<MeleeMode>(Settings::Aimbot::melee);
		float maxFov = mode == MeleeMode::LEGIT ? 90.0f : 180.0f;
		float smallestFov = maxFov;

		Vector viewAngles, viewForward;
		interfaces::Engine->GetViewAngles(viewAngles);
		Math::AngleVectors(viewAngles, &viewForward);
		viewForward.Normalize();

		bool bCanHitTeammates = pWeapon->CanHitTeammates();
		bool bIsWrench = pWeapon->GetWeaponID() == TF_WEAPON_WRENCH;

		for (const EntityListEntry& entry : AimbotUtils::GetTargets(bCanHitTeammates, localTeam))
		{
			if (!bIsWrench && (entry.flags & EntityFlags::IsBuilding) && !(entry.flags & EntityFlags::IsEnemy))
				continue;

			CBaseEntity* enemy = entry.ptr;

			Vector hitPos = enemy->GetCenter();
			Vector dir = hitPos - shootPos;
			float distance = dir.Normalize();
			if (distance > range)
				continue;

			Vector angle = Math::CalcAngle(shootPos, hitPos);
			float fov = Math::CalcFov(viewAngles, angle);
			if (fov > maxFov || fov > smallestFov)
				continue;

			helper::engine::TraceHull(shootPos, shootPos + (dir * range), swingMins, swingMaxs, MASK_SHOT_HULL, &filter, &trace);

			if (!trace.DidHit() || trace.m_pEnt != enemy)
				continue;

			targetAngle = dir.ToAngle();
			target = enemy;
			smallestFov = fov;
		}

		if (target == nullptr)
			return;

		if (Settings::Aimbot::autoshoot)
			pCmd->buttons |= IN_ATTACK;

		if (helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
		{
			Vector angle = targetAngle;
			state.angle = angle;
			pCmd->viewangles = angle;

			if (Settings::Aimbot::mode == static_cast<int>(AimbotMode::SILENT))
				state.shouldSilent = true;
		}

		EntityList::m_pAimbotTarget = target;
		state.running = true;
	}

	std::string GetMeleeModeName()
	{
		switch(static_cast<MeleeMode>(Settings::Aimbot::melee))
		{
			case MeleeMode::NONE: return "None";
			case MeleeMode::LEGIT: return "Legit";
			case MeleeMode::RAGE: return "Rage";
			default: return "Invalid";
		}
	}
};
