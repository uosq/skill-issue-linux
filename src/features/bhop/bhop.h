#pragma once

#include "../../sdk/classes/player.h"
#include "../../sdk/definitions/cusercmd.h"

#include "../config/config.h"
#include "../feature.h"

BEGIN_CONFIG(bhop)

	ADD_CONFIG(enabled, "bhop enabled", false)

END_CONFIG()

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