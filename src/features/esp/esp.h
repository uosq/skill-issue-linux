#pragma once

class CTFPlayer;

namespace ESP
{
	void Init();
	void Reset();

	// Call on FRAME_NET_UPDATE_END
	void OnFrameStageNotify();
	void OnLevelShutdown();
	void OnlevelInitPostEntity();
	void OnImGui();
};