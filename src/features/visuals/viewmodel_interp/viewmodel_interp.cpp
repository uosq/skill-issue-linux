#include "viewmodel_interp.h"

Vector ViewmodelInterp::m_vecOldAngle = {};

void ViewmodelInterp::Run(Vector& angle)
{
	if (Settings::Misc::viewmodel_interp == 0.0f)
		return;

	CTFPlayer* pLocal = helper::engine::GetLocalPlayer();
	if (pLocal == nullptr || !pLocal->IsAlive())
		return;

	CTFWeaponBase* pWeapon = HandleAs<CTFWeaponBase*>(pLocal->GetActiveWeapon());
	if (pWeapon == nullptr)
		return;

	float frameTime = interfaces::GlobalVars->absolute_frametime;
	float alpha = 1.0f - expf(-Settings::Misc::viewmodel_interp * frameTime);

	m_vecOldAngle.x += Math::NormalizeAngle(angle.x - m_vecOldAngle.x) * alpha;
	m_vecOldAngle.z += Math::NormalizeAngle(angle.z - m_vecOldAngle.z) * alpha;
	m_vecOldAngle.y += Math::NormalizeAngle(angle.y - m_vecOldAngle.y) * alpha;

	angle = m_vecOldAngle;
}

void ViewmodelInterp::Init()
{
	m_vecOldAngle = Vector(0, 0, 0);
}