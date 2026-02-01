#pragma once

#include "../../../sdk/interfaces/interfaces.h"
#include "../../../sdk/helpers/helper.h"
#include "../../../settings.h"

class ViewmodelInterp
{
public:
	void Run(Vector& angle);
};

inline void ViewmodelInterp::Run(Vector& angle)
{
	if (g_Settings.misc.viewmodel_interp == 0.0f)
		return;

	CTFPlayer* pLocal = helper::engine::GetLocalPlayer();
	if (pLocal == nullptr || !pLocal->IsAlive())
		return;

	CTFWeaponBase* pWeapon = HandleAs<CTFWeaponBase*>(pLocal->GetActiveWeapon());
	if (pWeapon == nullptr)
		return;

	static Vector smooth;
	float frameTime = interfaces::GlobalVars->absolute_frametime;
	float alpha = 1.0f - expf(-g_Settings.misc.viewmodel_interp * frameTime);

	smooth.x += Math::NormalizeAngle(angle.x - smooth.x) * alpha;
	smooth.z += Math::NormalizeAngle(angle.z - smooth.z) * alpha;
	smooth.y += Math::NormalizeAngle(angle.y - smooth.y) * alpha;

	angle = smooth;
}

inline ViewmodelInterp g_ViewmodelInterp;