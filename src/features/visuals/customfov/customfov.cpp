#include "customfov.h"

float CustomFov::m_flFov    = 90.0f;
float CustomFov::m_flOldFov = 90.0f;

void CustomFov::Run(CTFPlayer *pLocal, CViewSetup *pView)
{
	static ConVar *fov_desired = interfaces::Cvar->FindVar("fov_desired");

	float target_fov = Settings::Misc.customfov_enabled ? Settings::Misc.customfov : fov_desired->GetFloat();

	if (pLocal->InCond(TF_COND_ZOOMED))
		target_fov = Settings::Misc.zoomedfov;

	m_flFov = Math::Lerp(m_flOldFov, target_fov, 0.2f);

	if (interfaces::Engine->IsTakingScreenshot())
	{
		pView->fov = fov_desired->GetFloat();
		return;
	}

	pView->fov = m_flFov;

	if (pLocal->IsAlive())
	{
		pLocal->m_iDefaultFOV() =
		Settings::Misc.customfov_enabled
		? Settings::Misc.customfov
		: fov_desired->GetFloat();
	}

	m_flOldFov = m_flFov;
}

float CustomFov::GetFov()
{
	return m_flFov;
}