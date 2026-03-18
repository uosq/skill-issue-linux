#include "triggerbot.h"
#include "autoairblast/autoairblast.h"
#include "autobackstab/autobackstab.h"

#include "../ticks/ticks.h"

namespace Triggerbot
{
	static bool TraceEnemy(CTFPlayer* pLocal, CGameTrace& trace)
	{
		CTraceFilterHitscan filter;
		filter.pSkip = pLocal;

		Vector viewAngles, forward;
		interfaces::Engine->GetViewAngles(viewAngles);
		Math::AngleVectors(viewAngles, &forward);

		Vector start = pLocal->GetAbsOrigin() + pLocal->m_vecViewOffset();
		Vector end   = start + (forward * 8192);
		
		int localTeam = pLocal->m_iTeamNum();

		helper::engine::Trace(start, end, MASK_SHOT | CONTENTS_HITBOX, &filter, &trace);

		if (!trace.DidHit() || trace.m_pEnt == nullptr)
			return false;

		if (!AimbotUtils::IsValidEntity(trace.m_pEnt))
			return false;

		if (trace.m_pEnt->m_iTeamNum() == pLocal->m_iTeamNum())
			return false;

		return true;
	}

	void Hitscan(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd)
	{
		if (pLocal == nullptr || pWeapon == nullptr)
			return;

		if (!Settings::Trigger.hitscan)
			return;

		CGameTrace trace;
		if (!TraceEnemy(pLocal, trace))
			return;

		EntityList::m_pAimbotTarget = trace.m_pEnt;
		pCmd->buttons |= IN_ATTACK;
	}

	void ClassicTrigger(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd)
	{
		if (pLocal == nullptr || pWeapon == nullptr)
			return;

		if (!Settings::Trigger.hitscan)
			return;

		auto* pClassic = static_cast<CTFSniperRifleClassic*>(pWeapon);
		float chargedDamage = pClassic->m_flChargedDamage();

		static bool s_bCharging = false;

		CGameTrace trace;
		bool bOnTarget = TraceEnemy(pLocal, trace);

		if (bOnTarget)
		{
			EntityList::m_pAimbotTarget = trace.m_pEnt;

			if (chargedDamage > 0.0f)
			{
				pCmd->buttons &= ~IN_ATTACK;
				s_bCharging = false;
			}
			else
			{
				pCmd->buttons |= IN_ATTACK;
				s_bCharging = true;
			}
		}
		else
		{
			if (s_bCharging && chargedDamage == 0.0f)
			{
				pCmd->buttons |= IN_ATTACK;
			}
			else
			{
				// Either idle or charge is ready but we drifted off target –
				// reset state so the next on-target frame starts fresh.
				s_bCharging = false;
			}
		}
	}

	void Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd)
	{
		if (pLocal == nullptr || pWeapon == nullptr || pCmd == nullptr)
			return;

		if (!Settings::Trigger.key->IsActive())
			return;

		// The Classic needs its own charge/release logic before the generic
		// hitscan path, because pressing IN_ATTACK merely starts charging
		// rather than firing a shot.
		if (pWeapon->GetWeaponID() == TF_WEAPON_SNIPERRIFLE_CLASSIC)
		{
			ClassicTrigger(pLocal, pWeapon, pCmd);
		}
		else if (Settings::Trigger.hitscan && pWeapon->IsHitscan())
		{
			Hitscan(pLocal, pWeapon, pCmd);
		}

		if (Settings::Trigger.autobackstab != static_cast<int>(AutoBackstabMode::NONE) && pWeapon->IsMelee())
			AutoBackstab::Run(pLocal, pWeapon, pCmd, &TickManager::m_bSendPacket);

		if (Settings::Trigger.autoairblast != static_cast<int>(AutoBackstabMode::NONE) && pWeapon->GetWeaponID() == TF_WEAPON_FLAMETHROWER)
			AutoAirblast::Run(pLocal, pWeapon, pCmd, &TickManager::m_bSendPacket);
	}
}
