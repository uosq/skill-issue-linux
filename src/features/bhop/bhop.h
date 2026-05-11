#pragma once

#include "../../sdk/classes/player.h"
#include "../../sdk/definitions/cusercmd.h"

#include "../../settings/settings.h"
#include "../feature.h"

class Bhop
{
public:
	void Run(CTFPlayer *pLocal, CUserCmd *pCmd);
	void Reset();

private:
	bool m_bJumpReleased{false};
	bool m_bUsedAirJump{false};
};

DECLARE_FEATURE(Bhop, bhop)