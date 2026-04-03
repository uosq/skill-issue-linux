#pragma once

#include "../../sdk/classes/player.h"
#include "../../sdk/definitions/cusercmd.h"
#include "../../settings/settings.h"

namespace Bhop
{
	extern bool m_bJumpReleased;
	extern bool m_bUsedAirJump;

	void Run(CTFPlayer *pLocal, CUserCmd *pCmd);
	void Reset();
} // namespace Bhop