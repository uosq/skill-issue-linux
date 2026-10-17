#include "../features/hookmanager/hookmanager.hpp"
#include "../features/misc/misc.h"

INIT_HOOK(CL_CheckForPureServerWhitelist, void, (void *&pFilesToReload), "engine.so", "83 3D ? ? ? ? 01 7E ? 80 3D ? ? ? ? 00 75")
{
	if (config::sv_pure_bypass::enabled.Get())
		return;

	original_CL_CheckForPureServerWhitelist(pFilesToReload);
}