#include "viewmodel_aim.h"

#include "../../aimbot/aimbot.h"

void ViewmodelAim::Run(Vector &angle)
{
	if (!config::aimbot::viewmodel_aim.Get())
		return;

	constexpr float DURATION = 0.5f;

	if (features::aimbot.IsRunning() && interfaces::GlobalVars && interfaces::GlobalVars->curtime)
		m_flStopTime = interfaces::GlobalVars->curtime + DURATION;

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