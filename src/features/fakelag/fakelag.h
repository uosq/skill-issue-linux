#pragma once

#include "../../sdk/definitions/cusercmd.h"

namespace FakeLag
{
	void Run(CUserCmd* pCmd);

	extern bool m_bFakeLagging;
	extern int m_iChoked;
	extern int m_iGoal;
}