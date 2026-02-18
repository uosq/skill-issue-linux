#include "customfov.h"

float CustomFov::m_flFov = 90.0f;
float CustomFov::m_flOldFov = 90.0f;

void CustomFov::Run(CTFPlayer* pLocal, CViewSetup* pView)
{
	static ConVar* fov_desired = interfaces::Cvar->FindVar("fov_desired");
	static ConVar* default_fov = interfaces::Cvar->FindVar("default_fov");

	if (Settings::Misc::customfov_enabled)
		m_flFov = Settings::Misc::customfov;
	else
		m_flFov = fov_desired->GetFloat();

	if (pLocal->InCond(TF_COND_ZOOMED))
		m_flFov = TF_WEAPON_ZOOM_FOV;

	float delta = m_flFov - m_flOldFov;
	m_flFov = Math::Lerp(m_flOldFov, m_flFov, 0.2f);

	if (interfaces::Engine->IsTakingScreenshot())
		pView->fov = fov_desired->GetFloat();

	/*
	note to me
	research how tf2 does sniper's zoom
	this doesn't feel right
	*/

	if (pLocal->IsAlive() && pView->fov >= fov_desired->GetFloat())
	{
		pLocal->m_iDefaultFOV() = m_flFov;
		pLocal->m_iFOV() = m_flFov;
	}

	// bro this is so convenient holy shit
	pView->fov = m_flOldFov = m_flFov;
}

float CustomFov::GetFov()
{
	return m_flFov;
}