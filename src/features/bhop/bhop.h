#pragma once

#include "../../sdk/classes/player.h"
#include "../../sdk/definitions/cusercmd.h"

#include "../config/config.h"
#include "../feature.h"

BEGIN_CONFIG(bhop)

	ADD_CONFIG(enabled, "bhop enabled", false)
	ADD_CONFIG(maxchance, "bhop chance", 100)
	ADD_CONFIG(perfect, "bhop perfect", false)

END_CONFIG()

struct Bhop
{
	void Run(CTFPlayer *pLocal, CUserCmd *pCmd);
	void Reset();

	bool m_bLastWantsJump = false;
	bool m_bJumpReleased = false;
	bool m_bUsedAirJump = false;
	int m_iJumps = 0;
};

DECLARE_FEATURE(Bhop, bhop)