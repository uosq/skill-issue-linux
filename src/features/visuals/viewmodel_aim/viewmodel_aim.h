#pragma once

#include "../../../features/aimbot/aimbot.h"
#include "../../../sdk/interfaces/interfaces.h"
#include "../../../sdk/helpers/helper.h"
#include "../../../settings.h"

class ViewmodelAim
{
private:
	float m_flStopTime = 0.0f;

public:
	void Run(Vector& angle);
	void ResetStopTime();
	float GetStopTime();
};

inline void ViewmodelAim::Run(Vector& angle)
{
	if (!g_Settings.aimbot.viewmodelaim)
		return;

	constexpr float interval = 0.5f;

	if (Aimbot::IsRunning() && interfaces::GlobalVars && interfaces::GlobalVars->curtime)
		m_flStopTime = interfaces::GlobalVars->curtime + interval;
	
	if (interfaces::GlobalVars && interfaces::GlobalVars->curtime && interfaces::GlobalVars->curtime < m_flStopTime)
		angle = Aimbot::GetAngle();
}

inline void ViewmodelAim::ResetStopTime()
{
	m_flStopTime = 0.0f;
}

inline float ViewmodelAim::GetStopTime()
{
	return m_flStopTime;
}

inline ViewmodelAim g_ViewmodelAim;