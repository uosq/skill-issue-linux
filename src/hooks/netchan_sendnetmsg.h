#pragma once

#include "../settings/settings.h"

#include "../libdetour/libdetour.h"
#include "../sdk/classes/entity.h"
#include "../sdk/classes/player.h"
#include "../sdk/definitions/inetchannel.h"
#include "../sdk/helpers/helper.h"
#include "../sdk/interfaces/interfaces.h"
#include "../sdk/signatures/signatures.h"

#include "../features/warp/warp.h"

// xref: NetMsg
ADD_SIG(CNetChannel_SendNetMsg, "engine.so", "55 48 89 E5 41 56 41 89 D6 41 55 41 89 CD")

inline detour_ctx_t SendNetMsg_ctx;
DETOUR_DECL_TYPE(bool, originalSendNetMsg, void *ptr, INetMessage &msg, bool bForceReliable, bool bVoice);

inline bool Hooked_SendNetMsg(void *ptr, INetMessage &msg, bool bForceReliable, bool bVoice)
{
	bool ret;
	DETOUR_ORIG_GET(&SendNetMsg_ctx, ret, originalSendNetMsg, ptr, msg, bForceReliable, bVoice);
	return ret;
}

inline void HookSendNetMsg()
{
	detour_init(&SendNetMsg_ctx, Sigs::CNetChannel_SendNetMsg.GetPointer(), (void *)&Hooked_SendNetMsg);
	detour_enable(&SendNetMsg_ctx);

#ifdef DEBUG
	constexpr Color_t color{100, 255, 100, 255};
	interfaces::Cvar->ConsoleColorPrintf(color, "CNetChan::SendNetMsg hooked\n");
#endif
}