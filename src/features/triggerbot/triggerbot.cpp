#include "triggerbot.h"
#include "autoairblast/autoairblast.h"

namespace Triggerbot
{
	void Hitscan(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd)
	{
		if (pLocal == nullptr || pWeapon == nullptr)
			return;

		if (!g_Settings.triggerbot.hitscan)
			return;

		CGameTrace trace;
		CTraceFilterHitscan filter;
		filter.pSkip = pLocal;

		Vector viewAngles, forward;
		interfaces::Engine->GetViewAngles(viewAngles);
		Math::AngleVectors(viewAngles, &forward);

		Vector start = pLocal->GetAbsOrigin() + pLocal->m_vecViewOffset();
		Vector end = start + (forward * 2048);

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

	void Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, bool* pSendPacket)
	{
		if (pLocal == nullptr || pWeapon == nullptr || pCmd == nullptr)
			return;

		if (!g_Settings.triggerbot.enabled)
			return;

		ButtonCode_t key = interfaces::InputSystem->StringToButtonCode(g_Settings.triggerbot.key.c_str());
		if (key != BUTTON_CODE_INVALID && !interfaces::InputSystem->IsButtonDown(key))
			return;

		if (g_Settings.triggerbot.hitscan && pWeapon->IsHitscan())
			Hitscan(pLocal, pWeapon, pCmd);

                if (g_Settings.triggerbot.autobackstab != AutoBackstabMode::NONE && pWeapon->IsMelee())
			g_Autobackstab.Run(pLocal, pWeapon, pCmd, pSendPacket);

		if (g_Settings.triggerbot.autoairblast && pWeapon->GetWeaponID() == TF_WEAPON_FLAMETHROWER)
			g_AutoAirblast.Run(pLocal, pWeapon, pCmd);
	}
}