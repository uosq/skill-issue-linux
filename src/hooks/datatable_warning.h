#pragma once

#include "../features/warp/warp.h"
#include "../libdetour/libdetour.h"

// sig 55 49 89 FA 48 89 E5 41 54 48 81 EC D8 10 00 00 84 C0 48 89 B5 48 FF FF FF 48 89 95 50 FF FF FF 48 89 8D 58 FF FF FF 4C 89 85 60 FF FF FF 4C 89 8D 68 FF FF FF 74 ? 0F 29 85 70 FF FF FF 0F 29 4D 80 0F 29 55 90 0F 29 5D A0 0F 29 65 B0 0F 29 6D C0 0F 29 75 D0 0F 29 7D E0 4C 8D A5 40 EF FF FF

DETOUR_DECL_TYPE(void, DataTable_Warning, const char* pInMessage, ...);
inline detour_ctx_t warning_ctx;

inline void Hooked_DataTable_Warning(const char* pInMessage, ...)
{
	/*
	do nothing
	*/
}

inline void Hook_DataTable_Warning(void)
{
	//xref: DataTable warning: %s
	void* original = sigscan_module("engine.so", "55 49 89 FA 48 89 E5 41 54 48 81 EC D8 10 00 00 84 C0 48 89 B5 48 FF FF FF 48 89 95 50 FF FF FF 48 89 8D 58 FF FF FF 4C 89 85 60 FF FF FF 4C 89 8D 68 FF FF FF 74 ? 0F 29 85 70 FF FF FF 0F 29 4D 80 0F 29 55 90 0F 29 5D A0 0F 29 65 B0 0F 29 6D C0 0F 29 75 D0 0F 29 7D E0 4C 8D A5 40 EF FF FF");

	detour_init(&warning_ctx, original, (void*)&Hooked_DataTable_Warning);
	if (!detour_enable(&warning_ctx))
	{
		interfaces::Cvar->ConsolePrintf("Failed to hook DataTable_Warning\n");
		return;
	}

	#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("DataTable_Warning hooked\n");
	#endif
}