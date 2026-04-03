#pragma once

#include "../../sdk/classes/player.h"
#include <vector>

namespace Spectators
{
	// call from FrameStageNotify
	void RunMain(CTFPlayer *pLocal);
	// call from ImGui
	void DrawList();
	bool IsSpectated(CTFPlayer *pTarget, std::vector<CTFPlayer *> &out);
	void Reset();
	void OnLevelInitPostEntity();
	void OnLevelShutdown();
	bool IsLocalPlayerSpectated();
} // namespace Spectators