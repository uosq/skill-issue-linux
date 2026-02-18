#pragma once

#include "../features/warp/warp.h"
#include "../libdetour/libdetour.h"

DETOUR_DECL_TYPE(bool, Interpolate, CBaseEntity* self, float currentTime);
inline detour_ctx_t interpolate_ctx;

inline bool Hooked_Interpolate(CBaseEntity* self, float currentTime)
{
	if (Warp::m_bShifting || Warp::m_bRecharging)
		return true;

	bool retVal;
	DETOUR_ORIG_GET(&interpolate_ctx, retVal, Interpolate, self, currentTime);
	return retVal;
}

inline void Hook_Interpolate(void)
{
	//xref: C_BaseAnimating::Interpolate
	void* original = sigscan_module("client.so", "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 68 4C 8B AF B8 07 00 00");

	detour_init(&interpolate_ctx, original, (void*)&Hooked_Interpolate);
	if (!detour_enable(&interpolate_ctx))
	{
		interfaces::Cvar->ConsolePrintf("Failed to hook CBaseAnimating::Interpolate\n");
		return;
	}

	#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("CBaseAnimating::Interpolate hooked\n");
	#endif
}