#include "../features/warp/warp.h"
#include "../features/entitylist/entitylist.h"

#include "../features/hookmanager/hookmanager.hpp"

//xref: C_BaseAnimating::Interpolate
INIT_HOOK(Interpolate, bool, (void* self, float currentTime), "client.so", "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 68 4C 8B AF B8 07 00 00")
{
	if (features::warp.IsShifting() || features::warp.IsRecharging())
	{
		auto pLocal = features::entities.GetLocal();

		if (pLocal && pLocal == self)
			return true;
	}

	return original_Interpolate(self, currentTime);
}