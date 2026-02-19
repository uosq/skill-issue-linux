#pragma once

#include "../../sdk/interfaces/interfaces.h"

namespace TickManager
{
	extern bool m_bSendPacket;
	extern int m_iChokedCommands;

	void CL_Move(float accumulated_extra_samples, bool bFinalTick);
	void CL_SendMove(void);
	void Post_CreateMove(int sequence_number);

	void Lua_CreateMove_Callback(CUserCmd* pCmd);
	void Init(void);

	// call in CL_Move
	void Run(float accumulated_extra_samples, bool bFinalTick);
}