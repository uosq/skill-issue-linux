#pragma once

#include "../../feature.h"
#include "../../config/config.h"

BEGIN_CONFIG(customfov)

	ADD_CONFIG(enabled, "custom fov enabled", false)
	ADD_CONFIG(unzoomed, "custom fov unzoomed", 90.0f)
	ADD_CONFIG(zoomed, "custom fov zoomed", 20.0f)

END_CONFIG()

class CTFPlayer;
class CViewSetup;

class CCustomFov
{
public:
	void OnOverrideView(CTFPlayer* pLocal, CViewSetup* pView);
	float GetTargetFov(CTFPlayer* pLocal);
};

DECLARE_FEATURE(CCustomFov, customfov)