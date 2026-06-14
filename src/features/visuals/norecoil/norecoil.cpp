#include "norecoil.h"

#include "../../entitylist/entitylist.h"
#include "../../spectators/spectators.h"

float NoRecoil::GetScale()
{
	float value = config::norecoil::scale.Get() / 100.0f;
	return 1.0f - std::clamp(value, 0.0f, 1.0f);
}

void NoRecoil::RunOverrideView(CTFPlayer *pLocal, CViewSetup *pView)
{
	if (!pLocal->IsAlive())
		return;

	if (!config::norecoil::enabled.Get())
		return;

	if (interfaces::CInput->CAM_IsThirdPerson())
		return;

	Vector punchAngle = pLocal->m_vecPunchAngle();
	if (punchAngle.IsZero())
		return;

	pView->angles -= punchAngle * GetScale();
}

void NoRecoil::RunCreateMove(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd)
{
	if (!config::norecoil::enabled.Get())
		return;

	if (!helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
		return;

	Vector punchAngle = pLocal->m_vecPunchAngle();
	if (punchAngle.IsZero())
		return;

	// can't ignore
	if (!config::norecoil::ignore_spectators.Get())
	{
		int spectator_amount = 0;

		if (features::spectators.IsLocalPlayerSpectated(spectator_amount))
			return;
	}

	pCmd->viewangles -= punchAngle * GetScale();
}

void NoRecoil::RunCalcViewModelView(Vector &angle)
{
	if (!config::norecoil::enabled.Get())
		return;

	CTFPlayer* pLocal = features::entities.GetLocal();
	if (pLocal == nullptr)
		return;

	Vector punchAngle = pLocal->m_vecPunchAngle();
	if (punchAngle.IsZero())
		return;

	angle -= punchAngle * GetScale();
}