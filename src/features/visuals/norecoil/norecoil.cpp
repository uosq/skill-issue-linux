#include "norecoil.h"

#include "../../entitylist/entitylist.h"
#include "../../../settings/settings.h"

void NoRecoil::RunOverrideView(CTFPlayer *pLocal, CViewSetup *pView)
{
	if (!pLocal->IsAlive())
		return;

	if (!Config.misc.packed.norecoil)
		return;

	if (interfaces::CInput->CAM_IsThirdPerson())
		return;

	Vector punchAngle = pLocal->m_vecPunchAngle();
	if (punchAngle.IsZero())
		return;

	pView->angles -= punchAngle;
}

void NoRecoil::RunCreateMove(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd)
{
	if (!Config.misc.packed.norecoil)
		return;

	if (!helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
		return;

	Vector punchAngle = pLocal->m_vecPunchAngle();
	if (punchAngle.IsZero())
		return;

	pCmd->viewangles -= punchAngle;
}

void NoRecoil::RunCalcViewModelView(Vector &angle)
{
	if (!Config.misc.packed.norecoil)
		return;

	CTFPlayer* pLocal = features::entities.GetLocal();
	if (pLocal == nullptr)
		return;

	Vector punchAngle = pLocal->m_vecPunchAngle();
	if (punchAngle.IsZero())
		return;

	angle -= punchAngle;
}