#include "calcviewmodelbobhelper.h"

DETOUR_DECL_TYPE(float, CalcViewModelBobHelper, CTFPlayer *pPlayer, BobState_t *pBobState);
detour_ctx_t calcbobhelper;

float Hooked_CalcViewModelBobHelper(CTFPlayer *pPlayer, BobState_t *pBobState)
{
	if (!Settings::Misc.no_viewmodel_bob)
	{
		DETOUR_ORIG_CALL(&calcbobhelper, CalcViewModelBobHelper, pPlayer, pBobState);
		return 0.0f;
	}

	memset(pBobState, 0, sizeof(BobState_t));
	return 0.0f;
}

void Hook_CalcViewModelBobHelper()
{
	detour_init(&calcbobhelper, Sigs::CalcViewModelBobHelper.GetPointer(), (void *)&Hooked_CalcViewModelBobHelper);
	if (!detour_enable(&calcbobhelper))
		Logs::Error("Couldn't hook CalcViewModelBobHelper");
}