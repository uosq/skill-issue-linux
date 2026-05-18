#pragma once

#include "../../../features/aimbot/aimbot.h"
#include "../../../sdk/helpers/helper.h"
#include "../../../sdk/interfaces/interfaces.h"
#include "../../../settings/settings.h"

#include "../../feature.h"

class ViewmodelAim
{
public:
	void Init();
	void Run(Vector &angle);
	void ResetStopTime();
	float GetStopTime();

private:
	float m_flStopTime{0};
};

DECLARE_FEATURE(ViewmodelAim, viewmodel_aim)