#pragma once

#include "../feature.h"
#include "../config/config.h"

BEGIN_CONFIG(nopush)

	ADD_CONFIG(enabled, "nopush enabled", false)

END_CONFIG()

class INetMessage;

class CNoPush
{
public:
	void OnFrameStageNotify();
	void OnSendNetMsg(INetMessage& msg);
};

DECLARE_FEATURE(CNoPush, nopush)