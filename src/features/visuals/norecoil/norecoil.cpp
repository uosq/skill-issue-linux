#include "norecoil.h"

void NoRecoil::RunOverrideView(CTFPlayer* pLocal, CViewSetup* pView)
{
	if (!Settings::misc.norecoil)
		return;

	if (interfaces::CInput->CAM_IsThirdPerson())
		return;

	Vector punchAngle = pLocal->m_vecPunchAngle();
	if (punchAngle.IsZero())
		return;

	pView->angles -= punchAngle;
}

void NoRecoil::RunCreateMove(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd)
{
	if (!Settings::misc.norecoil)
		return;

	if (!helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
		return;

	Vector punchAngle = pLocal->m_vecPunchAngle();
	if (punchAngle.IsZero())
		return;

	pCmd->viewangles -= punchAngle;
}