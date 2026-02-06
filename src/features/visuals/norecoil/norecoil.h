#pragma once

#include "../../../sdk/helpers/helper.h"
#include "../../../settings.h"

namespace NoRecoil
{
	inline void Run(CTFPlayer* pLocal, CViewSetup* pView)
	{
		if (!g_Settings.misc.norecoil)
			return;

		if (interfaces::CInput->CAM_IsThirdPerson())
			return;

		Vector punchAngle = pLocal->m_vecPunchAngle();
		if (punchAngle.IsZero())
			return;

		pView->angles -= punchAngle;
	}

	inline void RunCreateMove(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd)
	{
		if (!g_Settings.misc.norecoil)
			return;

		if (g_Settings.misc.norecoil_hide && !helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
			return;

		Vector punchAngle = pLocal->m_vecPunchAngle();
		if (punchAngle.IsZero())
			return;

		pCmd->viewangles -= punchAngle;
	}
}