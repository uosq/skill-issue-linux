#include "viewmodel_aim.h"

float ViewmodelAim::m_flStopTime = 0;

void ViewmodelAim::Run(Vector& angle)
{
	if (!Settings::Aimbot::viewmodelaim)
		return;

	constexpr float interval = 0.5f;

	if (Aimbot::IsRunning() && interfaces::GlobalVars && interfaces::GlobalVars->curtime)
		m_flStopTime = interfaces::GlobalVars->curtime + interval;
	
	if (interfaces::GlobalVars && interfaces::GlobalVars->curtime && interfaces::GlobalVars->curtime < m_flStopTime)
		angle = Aimbot::GetAngle();
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