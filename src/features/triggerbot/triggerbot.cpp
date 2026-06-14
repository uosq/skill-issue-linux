#include "triggerbot.h"
#include "autoairblast/autoairblast.h"

#include "../ticks/ticks.h"
#include "autobackstab/autobackstab.h"

void Triggerbot::Hitscan(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd)
{
	if (pLocal == nullptr || pWeapon == nullptr)
		return;

	CGameTrace trace;
	CTraceFilterHitscan filter;
	filter.pSkip = pLocal;

	Vector viewAngles, forward;
	interfaces::Engine->GetViewAngles(viewAngles);
	Math::AngleVectors(viewAngles, &forward);

	Vector start  = pLocal->GetAbsOrigin() + pLocal->m_vecViewOffset();
	Vector end    = start + (forward * 8192);

	int localTeam = pLocal->m_iTeamNum();

	helper::engine::Trace(start, end, MASK_SHOT | CONTENTS_HITBOX, &filter, &trace);

	if (!trace.DidHit() || trace.m_pEnt == nullptr)
		return;

	if (!AimbotUtils::IsValidEntity(trace.m_pEnt))
		return;

	if (trace.m_pEnt->m_iTeamNum() == localTeam)
		return;

	helper::localplayer::Shoot(pLocal, pWeapon, pCmd, trace.m_pEnt);
	features::entities.SetAimbotTarget(trace.m_pEnt);
}

void Triggerbot::Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd)
{
	if (pLocal == nullptr || pWeapon == nullptr || pCmd == nullptr)
		return;

	if (helper::localplayer::IsChatOpen())
		return;

	if (helper::engine::IsGameUIVisible())
		return;

	if (helper::engine::IsConsoleVisible())
		return;

	if (!config::trigger::key.Get().IsActive())
		return;

	if (config::trigger::hitscan.Get() && pWeapon->IsHitscan())
		Hitscan(pLocal, pWeapon, pCmd);

	if (config::autobackstab::enabled.Get() != static_cast<int>(GenericMode::NONE) && pWeapon->IsMelee())
		features::autobackstab.Run(pLocal, pWeapon, pCmd, &features::ticks.GetSendPacket());

	if (config::autoairblast::enabled.Get() != static_cast<int>(GenericMode::NONE) &&
	    pWeapon->GetWeaponID() == TF_WEAPON_FLAMETHROWER)
		features::autoairblast.Run(pLocal, pWeapon, pCmd, &features::ticks.GetSendPacket());
}