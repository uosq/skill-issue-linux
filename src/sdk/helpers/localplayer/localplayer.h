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

		template <typename... Args>
    		void ChatPrintf(int iPlayerIndex, int iFilter, const char *fmt, Args&&... args)
		{
			// xref: Could not find Hud Element: %s\n
			using GetHudElementFn = void*(*)(void* gHUD, const char* name);
			static GetHudElementFn GET_HUDLEMENT = reinterpret_cast<GetHudElementFn>(sigscan_module("client.so", "55 48 89 E5 41 57 41 56 41 55 41 54 49 89 F4 53 48 83 EC 08 8B 47 30"));

			void* hud_chat = GET_HUDLEMENT(interfaces::gHUD, "CHudChat");

			if (!hud_chat)
				return;

			/*
			xref: #Achievement_Earned

				uVar11 = (**(code **)(*DAT_03027568 + 0x10))(DAT_03027568,"#Achievement_Earned");
				FUN_016b30b0(&local_838,uVar11,2,local_1038,lVar20);
				FUN_01fda860(&local_838,local_c38,0x80);
		=======>      		(**(code **)(*hudChat + 0xb0))(hudChat,uVar4,8,&DAT_0235922d,local_c38);
				return;

			0xb0 / sizeof(uintptr_t) = 22
			inside CHudChat's vtable -> 22 index is ChatPrintf
			*/
			using ChatPrintfFn = void (*)(void*, int, int, const char * ...);
			static ChatPrintfFn chatprintf = reinterpret_cast<ChatPrintfFn>(sigscan_module("client.so", "55 48 89 E5 41 57 41 56 41 55 49 89 FD 41 54 41 89 F4 53 89 D3 48 89 CA"));

			chatprintf(hud_chat, iPlayerIndex, iFilter, fmt, std::forward<Args>(args)...);
		}

		// returns if we are shooting
		bool Shoot(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, CBaseEntity* pTarget = nullptr);
	} // namespace localplayer
} // namespace helper