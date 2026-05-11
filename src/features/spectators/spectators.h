#pragma once

#include <mutex>
#include <vector>

#include "../../sdk/classes/player.h"

#include "../feature.h"

struct SpectatorData
{
	std::string name;
	bool isFirstPerson;
};

class Spectators
{
public:
	// call from FrameStageNotify
	void OnFrameStageNotify();
	// call from ImGui
	void DrawList();
	bool IsSpectated(CTFPlayer *pTarget, std::vector<CTFPlayer*> &out);
	void Reset();
	void OnLevelInitPostEntity();
	void OnLevelShutdown();
	bool IsLocalPlayerSpectated(int& amount);

private:
	bool s_bSpectated{false};
	std::mutex s_Mutex;
	std::vector<SpectatorData> s_vCachedSpectatorList;
};

DECLARE_FEATURE(Spectators, spectators)