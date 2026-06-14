#pragma once

#include <cstdint>

#include "../feature.h"

#include "../config/config.h"

BEGIN_CONFIG(spyalert)

	ADD_CONFIG(enabled, "spyalert enabled", false)

END_CONFIG()

class CTFPlayer;
struct ImDrawList;

class SpyAlert
{
public:
	void OnFrameStageNotify();
	void OnImGui(ImDrawList* pDraw);
	bool IsSpyNear();

private:
	uint8_t s_iSpyStatus{0};
};

DECLARE_FEATURE(SpyAlert, spyalert)