#pragma once

#include "../../../sdk/helpers/helper.h"
#include "../../../sdk/interfaces/interfaces.h"
#include "../../../settings/settings.h"

namespace CustomFov
{
	extern float m_flFov;
	extern float m_flOldFov;

	void Run(CTFPlayer *pLocal, CViewSetup *pView);
	float GetFov();
}; // namespace CustomFov