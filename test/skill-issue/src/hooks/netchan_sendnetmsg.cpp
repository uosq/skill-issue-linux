#include "netchan_sendnetmsg.h"

#include "../libdetour/libdetour.h"

DETOUR_DECL_TYPE(bool, originalSendNetMsg, void *ptr, INetMessage &msg, bool bForceReliable, bool bVoice);
detour_ctx_t SendNetMsg_ctx;

bool Hooked_SendNetMsg(void *ptr, INetMessage &msg, bool bForceReliable, bool bVoice)
{
	bool ret;
	DETOUR_ORIG_GET(&SendNetMsg_ctx, ret, originalSendNetMsg, ptr, msg, bForceReliable, bVoice);
	return ret;
}

void HookSendNetMsg()
{
	detour_init(&SendNetMsg_ctx, Sigs::CNetChannel_SendNetMsg.GetPointer(), (void *)&Hooked_SendNetMsg);
	detour_enable(&SendNetMsg_ctx);

#ifdef DEBUG
	constexpr Color_t color{100, 255, 100, 255};
	interfaces::Cvar->ConsoleColorPrintf(color, "CNetChan::SendNetMsg hooked\n");
#endif
}