#include "../sdk/definitions/inetmessage.h"

#include "../features/hookmanager/hookmanager.hpp"
#include "../features/nopush/nopush.h"

// xref: NetMsg
INIT_HOOK(SendNetMsg, bool, (void* ptr, INetMessage &msg, bool bForceReliable, bool bVoice), "engine.so", "55 48 89 E5 41 56 41 89 D6 41 55 41 89 CD")
{
	switch(msg.GetType())
	{
		case net_Tick:
		{
			features::nopush.OnSendNetMsg(msg);
			break;
		}

		default:
		break;
	}

	return original_SendNetMsg(ptr, msg, bForceReliable, bVoice);
}