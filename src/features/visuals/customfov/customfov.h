#pragma once

#include "../../feature.h"

class CTFPlayer;
class CViewSetup;

class CCustomFov
{
public:
	void OnOverrideView(CTFPlayer* pLocal, CViewSetup* pView);
	float GetTargetFov(CTFPlayer* pLocal);
};

DECLARE_FEATURE(CCustomFov, customfov)