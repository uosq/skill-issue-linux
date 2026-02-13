#pragma once

#include "../sdk/interfaces/interfaces.h"
#include "../sdk/classes/entity.h"
#include "../sdk/classes/player.h"
#include "../sdk/helpers/helper.h"
#include "../settings.h"
#include "../libdetour/libdetour.h"
#include "../sdk/definitions/inetchannel.h"

#include "../features/lua/hookmgr.h"
#include "../features/lua/api.h"
#include "../features/lua/classes.h"

inline detour_ctx_t SendNetMsg_ctx;
DETOUR_DECL_TYPE(bool, originalSendNetMsg, void* ptr, INetMessage& msg, bool bForceReliable, bool bVoice);

inline bool Hooked_SendNetMsg(void* ptr, INetMessage& msg, bool bForceReliable, bool bVoice)
{
	/*if (msg.GetType() == net_Tick)
	{
		unsigned char bf[4096];
		bf_write write(bf, sizeof(bf));
		msg.WriteToBuffer(write);

		bf_read reader(bf, sizeof(bf));
		reader.Seek(NETMSG_TYPE_BITS);
		int m_nTick = reader.ReadLong();
		float m_flHostFrameTime = (float)reader.ReadUBitLong(16) / NET_TICK_SCALEUP;
		float m_flHostFrameTimeStdDeviation = (float)reader.ReadUBitLong(16) / NET_TICK_SCALEUP;
		//interfaces::Cvar->ConsolePrintf("SendNetNest ToString - %s\n", msg.ToString());
		interfaces::Cvar->ConsolePrintf("SendNetMsg - tick: %i, frametime: %f, deviation: %f\n", m_nTick, m_flHostFrameTime, m_flHostFrameTimeStdDeviation);
	}*/

	if (LuaHookManager::HasHooks("SendNetMsg"))
	{
		LuaClasses::NetMessageLua::push_netmessage(Lua::m_luaState, &msg);
		LuaHookManager::Call(Lua::m_luaState, "SendNetMsg", 1);
	}

	bool ret;
	DETOUR_ORIG_GET(&SendNetMsg_ctx, ret, originalSendNetMsg, ptr, msg, bForceReliable, bVoice);
	return ret;
}

inline void HookSendNetMsg()
{
	// xref: NetMsg
	void* original = sigscan_module("engine.so", "55 48 89 E5 41 56 41 89 D6 41 55 41 89 CD");
	detour_init(&SendNetMsg_ctx, original, (void*)&Hooked_SendNetMsg);
	detour_enable(&SendNetMsg_ctx);

	constexpr Color_t color{100, 255, 100, 255};
	interfaces::Cvar->ConsoleColorPrintf(color, "CNetChan::SendNetMsg hooked\n");
}