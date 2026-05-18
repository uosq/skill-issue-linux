#include "viewmodel_aim.h"

void ViewmodelAim::Run(Vector &angle)
{
	if (!Config.aimbot.packed.viewmodelaim)
		return;

	constexpr float interval = 0.5f;

	if (features::aimbot.IsRunning() && interfaces::GlobalVars && interfaces::GlobalVars->curtime)
		m_flStopTime = interfaces::GlobalVars->curtime + interval;

	if (interfaces::GlobalVars && interfaces::GlobalVars->curtime && interfaces::GlobalVars->curtime < m_flStopTime)
		angle = features::aimbot.GetAngle();
}

void ViewmodelAim::ResetStopTime()
{
	m_flStopTime = 0.0f;
}

float ViewmodelAim::GetStopTime()
{
	return m_flStopTime;
}

void ViewmodelAim::Init()
{
	m_flStopTime = 0;
}