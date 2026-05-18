#pragma once

#include "../../sdk/interfaces/interfaces.h"
#include "../feature.h"

class TickManager
{
public:
	void Init(void);

	// call in CL_Move
	void Run(float accumulated_extra_samples, bool bFinalTick);

	uint8_t GetChokedCommands();
	bool& GetSendPacket();
private:
	void CL_Move(float accumulated_extra_samples, bool bFinalTick);
	void CL_SendMove(void);
	void Post_CreateMove(int sequence_number);

	bool m_bSendPacket;
	uint8_t m_iChokedCommands;
};

DECLARE_FEATURE(TickManager, ticks)