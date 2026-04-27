#pragma once

class CTFPlayer;
struct ImDrawList;

namespace SpyAlert
{
	void OnFrameStageNotify();
	void OnImGui(ImDrawList* pDraw);
	bool IsSpyNear();
}