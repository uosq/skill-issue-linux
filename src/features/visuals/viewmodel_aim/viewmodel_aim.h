#pragma once

#include "../../../features/aimbot/aimbot.h"
#include "../../../sdk/interfaces/interfaces.h"
#include "../../../sdk/helpers/helper.h"
#include "../../../settings.h"

namespace ViewmodelAim
{
	extern float m_flStopTime;

	void Init();
	void Run(Vector& angle);
	void ResetStopTime();
	float GetStopTime();
};