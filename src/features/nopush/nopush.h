#pragma once

#include "../feature.h"

class INetMessage;

class CNoPush
{
public:
	void OnFrameStageNotify();
	void OnSendNetMsg(INetMessage& msg);
};

DECLARE_FEATURE(CNoPush, nopush)