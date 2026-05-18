#pragma once

#include "../../../sdk/helpers/helper.h"
#include "../../../sdk/interfaces/interfaces.h"

#include "../../../settings/settings.h"
#include "../../feature.h"

class CustomFov
{
public:
	void Run(CTFPlayer *pLocal, CViewSetup *pView);
	float GetFov();

private:
	float m_flFov{90};
	float m_flOldFov{90};
};

DECLARE_FEATURE(CustomFov, customfov)