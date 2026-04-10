#include "cbaseentity_baseinterpolatepart1.h"

#include "../features/entitylist/entitylist.h"

DETOUR_DECL_TYPE(int, BaseInterpolatePart1, CBaseEntity *self, float *currentTime, Vector *oldOrigin, QAngle *oldAngles, Vector *oldVel, int *bNoMoreChanges);
detour_ctx_t interp_ctx;

int Hooked_BaseInterpolatePart1(CBaseEntity *self, float *currentTime, Vector *oldOrigin, QAngle *oldAngles, Vector *oldVel, int *bNoMoreChanges)
{
	const int INTERPOLATE_STOP = 0;

	if (self == nullptr || currentTime == nullptr || oldOrigin == nullptr || oldAngles == nullptr ||
	    oldVel == nullptr || bNoMoreChanges == nullptr)
		return INTERPOLATE_STOP;

	if (Warp::m_bShifting || Warp::m_bRecharging)
	{
		auto pLocal = EntityList::GetLocal();
		if (pLocal && pLocal == self)
			return INTERPOLATE_STOP;
	}

	int retVal;
	DETOUR_ORIG_GET
	(
		&interp_ctx,
		retVal,
		BaseInterpolatePart1,
		self,
		currentTime,
		oldOrigin, oldAngles, oldVel,
		bNoMoreChanges
	);
	return retVal;
}

void Hook_BaseInterpolatePart1(void)
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