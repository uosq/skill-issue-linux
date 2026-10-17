#include "../features/hookmanager/hookmanager.hpp"
#include "../features/ticks/ticks.h"

INIT_HOOK(CL_Move, void, (float accumulated_extra_samples, bool bFinalTick), "engine.so", "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 78 83 3D ? ? ? ? 01")
{
	features::ticks.Run(accumulated_extra_samples, bFinalTick);
}