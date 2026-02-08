#pragma once

// ExecuteStringCommand
// engine.so 55 48 89 E5 41 57 41 56 4C 8D B5 C0 F9 FF FF

#include "../libdetour/libdetour.h"

#include "../features/lua/hookmgr.h"
#include "../features/lua/api.h"
#include "../features/lua/classes.h"
#include <string>

DETOUR_DECL_TYPE(bool, ExecuteStringCommand, void* self, const char* pCommandString);
inline detour_ctx_t execstringcmd_ctx;

inline bool HookedExecuteStringCommand(void* self, const char* pCommandString)
{
	std::string cmd = pCommandString;

	if (LuaHookManager::HasHooks("ExecStringCmd"))
	{
		LuaClasses::StringCmdLua::push(Lua::m_luaState, cmd);
		LuaHookManager::Call(Lua::m_luaState, "ExecStringCmd", 1);
	}

	bool ret;
	DETOUR_ORIG_GET(&execstringcmd_ctx, ret, ExecuteStringCommand, self, cmd.c_str());

	return ret;
}

inline void Hook_ExecuteStringCommand()
{
	void* original = sigscan_module("engine.so", "55 48 89 E5 41 57 41 56 4C 8D B5 C0 F9 FF FF");
	detour_init(&execstringcmd_ctx, original, (void*)&HookedExecuteStringCommand);
	if (!detour_enable(&execstringcmd_ctx))
	{
		interfaces::Cvar->ConsolePrintf("Failed to hook CGameClient::ExecuteStringCommand\n");
		return;
	}

	interfaces::Cvar->ConsolePrintf("CGameClient::ExecuteStringCommand hooked\n");
}