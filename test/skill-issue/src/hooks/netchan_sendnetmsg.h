#pragma once

#include "../sdk/definitions/inetchannel.h"
#include "../sdk/signatures/signatures.h"

// xref: NetMsg
ADD_SIG(CNetChannel_SendNetMsg, "engine.so", "55 48 89 E5 41 56 41 89 D6 41 55 41 89 CD")

bool Hooked_SendNetMsg(void *ptr, INetMessage &msg, bool bForceReliable, bool bVoice);
void HookSendNetMsg();