#pragma once

#include "../../sdk/definitions/cusercmd.h"
#include "../../sdk/classes/player.h"
#include "../../settings/settings.h"

namespace Bhop
{
	extern bool m_bJumpReleased;
	extern bool m_bUsedAirJump;

	void Run(CTFPlayer* pLocal, CUserCmd* pCmd);
	void Reset();
}