#include "../features/warp/warp.h"
#include "../features/entitylist/entitylist.h"
#include "../features/hookmanager/hookmanager.hpp"

#include "../sdk/classes/entity.h"

// sig 55 48 89 E5 41 57 49 89 F7 41 56 49 89 CE 41 55 4D 89 CD
// args CBaseEntity* self, float &currentTime, Vector &oldOrigin, QAngle &oldAngles, Vector &oldVel, int &bNoMoreChanges

INIT_HOOK(BaseInterpolatePart1, int, (CBaseEntity *self, float *currentTime, Vector *oldOrigin, QAngle *oldAngles, Vector *oldVel, int *bNoMoreChanges), "client.so", "55 48 89 E5 41 57 49 89 F7 41 56 49 89 CE 41 55 4D 89 CD")
{
	constexpr int INTERPOLATE_STOP = 0;

	if (self == nullptr || currentTime == nullptr || oldOrigin == nullptr || oldAngles == nullptr ||
	    oldVel == nullptr || bNoMoreChanges == nullptr)
		return INTERPOLATE_STOP;

	if (features::warp.IsShifting() || features::warp.IsRecharging())
	{
		auto pLocal = features::entities.GetLocal();
		if (pLocal && pLocal == self)
			return INTERPOLATE_STOP;
	}

	return original_BaseInterpolatePart1(self, currentTime, oldOrigin, oldAngles, oldVel, bNoMoreChanges);
}