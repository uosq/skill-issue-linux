#include "triggerbot.h"
#include "autoairblast/autoairblast.h"

#include "../ticks/ticks.h"

namespace Triggerbot
{
	void Hitscan(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd)
	{
		if (pLocal == nullptr || pWeapon == nullptr)
			return;

		if (!Settings::Triggerbot::hitscan)
			return;

		CGameTrace trace;
		CTraceFilterHitscan filter;
		filter.pSkip = pLocal;

		Vector viewAngles, forward;
		interfaces::Engine->GetViewAngles(viewAngles);
		Math::AngleVectors(viewAngles, &forward);

		Vector start = pLocal->GetAbsOrigin() + pLocal->m_vecViewOffset();
		Vector end = start + (forward * 8192);

		int localTeam = pLocal->m_iTeamNum();

		helper::engine::Trace(start, end, MASK_SHOT | CONTENTS_HITBOX, &filter, &trace);

		if (!trace.DidHit() || trace.m_pEnt == nullptr)
			return;

		if (!AimbotUtils::IsValidEntity(trace.m_pEnt))
			return;

		if (trace.m_pEnt->m_iTeamNum() == localTeam)
			return;

		EntityList::m_pAimbotTarget = trace.m_pEnt;
		pCmd->buttons |= IN_ATTACK;
	}

	void Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd)
	{
		if (pLocal == nullptr || pWeapon == nullptr || pCmd == nullptr)
			return;

		if (!Settings::Triggerbot::enabled)
			return;

		ButtonCode_t key = interfaces::InputSystem->StringToButtonCode(Settings::Triggerbot::key.c_str());
		if (key != BUTTON_CODE_INVALID && !interfaces::InputSystem->IsButtonDown(key))
			return;

		if (Settings::Triggerbot::hitscan && pWeapon->IsHitscan())
			Hitscan(pLocal, pWeapon, pCmd);

                if (Settings::Triggerbot::autobackstab != static_cast<int>(AutoBackstabMode::NONE) && pWeapon->IsMelee())
			AutoBackstab::Run(pLocal, pWeapon, pCmd, &TickManager::m_bSendPacket);

		if (Settings::Triggerbot::autoairblast != static_cast<int>(AutoBackstabMode::NONE) && pWeapon->GetWeaponID() == TF_WEAPON_FLAMETHROWER)
			AutoAirblast::Run(pLocal, pWeapon, pCmd, &TickManager::m_bSendPacket);
	}
}