#pragma once

#include "../../classes/player.h"
#include "../../classes/weaponbase.h"
#include "../../definitions/hudbasechat.h"
#include <cstdarg>

namespace helper
{
	namespace localplayer
	{
		inline Vector LastAngle{};

		bool CanShoot(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, bool ignoreAttack = false);
		bool IsAttacking(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd);
		void ChatPrintf(int iPlayerIndex, int iFilter, const char *fmt);

		// returns if we are shooting
		bool ShootInternal(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, CBaseEntity* pTarget = nullptr);
		bool Shoot(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, CBaseEntity* pTarget = nullptr);
	} // namespace localplayer
} // namespace helper