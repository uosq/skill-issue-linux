#pragma once

#include "../features/warp/warp.h"
#include "../libdetour/libdetour.h"
#include "../sdk/helpers/helper.h"
#include "../sdk/signatures/signatures.h"

// sig 55 48 89 E5 41 57 49 89 F7 41 56 49 89 CE 41 55 4D 89 CD
// args CBaseEntity* self, float &currentTime, Vector &oldOrigin, QAngle &oldAngles, Vector &oldVel, int &bNoMoreChanges

DETOUR_DECL_TYPE(int, BaseInterpolatePart1, CBaseEntity *self, float *currentTime, Vector *oldOrigin, QAngle *oldAngles,
		 Vector *oldVel, int *bNoMoreChanges);
inline detour_ctx_t interp_ctx;

ADD_SIG(CBaseEntity_BaseInterpolateParte1, "client.so", "55 48 89 E5 41 57 49 89 F7 41 56 49 89 CE 41 55 4D 89 CD");

inline int Hooked_BaseInterpolatePart1(CBaseEntity *self, float *currentTime, Vector *oldOrigin, QAngle *oldAngles,
				       Vector *oldVel, int *bNoMoreChanges)
{
	const int INTERPOLATE_STOP = 0;

	if (self == nullptr || currentTime == nullptr || oldOrigin == nullptr || oldAngles == nullptr ||
	    oldVel == nullptr || bNoMoreChanges == nullptr)
		return INTERPOLATE_STOP;

	if (Warp::m_bShifting || Warp::m_bRecharging)
		return INTERPOLATE_STOP;

	int retVal;
	DETOUR_ORIG_GET(&interp_ctx, retVal, BaseInterpolatePart1, self, currentTime, oldOrigin, oldAngles, oldVel,
			bNoMoreChanges);
	return retVal;
}

inline void Hook_BaseInterpolatePart1(void)
{
	detour_init(&interp_ctx, Sigs::CBaseEntity_BaseInterpolateParte1.GetPointer(),
		    (void *)&Hooked_BaseInterpolatePart1);
	if (!detour_enable(&interp_ctx))
	{
		interfaces::Cvar->ConsolePrintf("Failed to hook CBaseEntity::BaseInterpolatePart1\n");
		return;
	}

#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("CBaseEntity::BaseInterpolatePart1 hooked\n");
#endif
}