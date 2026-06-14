#include "customfov.h"

#include "../../../sdk/interfaces/interfaces.h"
#include "../../../sdk/classes/player.h"

static float get_unzoomed_fov(CTFPlayer* local)
{
	assert(local);

	static ConVar* fov_desired = interfaces::Cvar->FindVar("fov_desired");

	// wtf
	if (!fov_desired)
		return 90.0f;

	if (local->IsAlive())
	{
		if (config::customfov::enabled.Get())
			return config::customfov::unzoomed.Get();
	}

	return fov_desired->GetFloat();
}

static float get_zoomed_fov(CTFPlayer* local)
{
	assert(local);

	if (local->IsAlive())
	{
		if (config::customfov::enabled.Get())
			return config::customfov::zoomed.Get();
	}

	return 20.0f;
}

float CCustomFov::GetTargetFov(CTFPlayer* local)
{
	if (local == nullptr)
		return 90.0f;

	bool in_zoom = local->InCond(TF_COND_ZOOMED);
	return in_zoom ? get_zoomed_fov(local) : get_unzoomed_fov(local);
}

void apply_viewmodel_fov(CTFPlayer* local, float fov)
{
	assert(local);
	
	if (!local->IsAlive())
		return;

	local->m_iDefaultFOV() = fov;
}

void CCustomFov::OnOverrideView(CTFPlayer* local, CViewSetup* view)
{
	if (!local || !view)
		return;

	float target_fov = GetTargetFov(local);

	// force shit
	if (interfaces::Engine->IsTakingScreenshot())
	{
		view->fov = target_fov;

		apply_viewmodel_fov(local, target_fov);

		return;
	}

	static float old_target_fov = 90.0f;
	const float deltatime = interfaces::GlobalVars->absolute_frametime;

	float interpolated_target_fov = Math::Lerp(old_target_fov, target_fov, 20.0f * deltatime);

	view->fov = interpolated_target_fov;
	old_target_fov = interpolated_target_fov;

	apply_viewmodel_fov(local, interpolated_target_fov);
}