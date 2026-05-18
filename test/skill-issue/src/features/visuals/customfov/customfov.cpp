#include "customfov.h"

void CustomFov::Run(CTFPlayer *pLocal, CViewSetup *pView)
{
	static ConVar *fov_desired = interfaces::Cvar->FindVar("fov_desired");

	bool enabled = Config.misc.packed.customfov_enabled && !interfaces::Engine->IsTakingScreenshot();
	float target_fov = enabled ? Config.misc.customfov : fov_desired->GetFloat();

	if (pLocal->InCond(TF_COND_ZOOMED))
		target_fov = enabled ? Config.misc.zoomedfov : 20.0f;

	m_flFov = Math::Lerp(m_flOldFov, target_fov, 0.2f);

	if (!pLocal->IsAlive() || interfaces::Engine->IsTakingScreenshot())
	{
		pView->fov = fov_desired->GetFloat();
		return;
	}

	pView->fov = m_flFov;

	if (pLocal->IsAlive())
	{
		pLocal->m_iDefaultFOV() =
		enabled
		? Config.misc.customfov
		: fov_desired->GetFloat();
	}

	m_flOldFov = m_flFov;
}

float CustomFov::GetFov()
{
	return m_flFov;
}