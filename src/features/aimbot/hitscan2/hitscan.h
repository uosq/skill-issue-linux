#pragma once

#include "../../../sdk/classes/player.h"
#include "../../../sdk/classes/entity.h"
#include "../../../sdk/classes/weaponbase.h"

enum class PreferredHitPoint
{
	HEAD = 0,
	PELVIS,
}; // end enum

namespace AimHitscan
{
	// main
	void RunMain(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd);
	void OnAttack(CTFWeaponBase* pWeapon, CUserCmd* pCmd);
	bool ApplyAim(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd);
	void Reset();

	// helpers
	CBaseEntity* GetTarget();
	PreferredHitPoint GetPreferredHitPoint(CTFPlayer* pLocal, CTFWeaponBase* pWeapon);
	bool CanWaitForCharge(CTFWeaponBase* pWeapon);
} // end namespace