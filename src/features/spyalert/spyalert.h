#pragma once

#include "../feature.h"
#include <cstdint>

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