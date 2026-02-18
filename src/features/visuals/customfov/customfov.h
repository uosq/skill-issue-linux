#pragma once

#include "../../../sdk/interfaces/interfaces.h"
#include "../../../sdk/helpers/helper.h"
#include "../../../settings/settings.h"

namespace CustomFov
{
	extern float m_flFov;
	extern float m_flOldFov;

	void Run(CTFPlayer* pLocal, CViewSetup* pView);
	float GetFov();
};