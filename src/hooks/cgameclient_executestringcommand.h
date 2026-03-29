#pragma once

// ExecuteStringCommand
// engine.so 55 48 89 E5 41 57 41 56 4C 8D B5 C0 F9 FF FF

#include "../libdetour/libdetour.h"
#include "../libsigscan.h"
#include "../sdk/interfaces/interfaces.h"

#include <string>

DETOUR_DECL_TYPE(bool, ExecuteStringCommand, void* self, const char* pCommandString);
inline detour_ctx_t execstringcmd_ctx;

inline bool HookedExecuteStringCommand(void* self, const char* pCommandString)
{
	//std::string cmd = pCommandString;

	bool ret;
	DETOUR_ORIG_GET(&execstringcmd_ctx, ret, ExecuteStringCommand, self, pCommandString);

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

	#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("CGameClient::ExecuteStringCommand hooked\n");
	#endif
}