#include "cl_move.h"

#include "../features/ticks/ticks.h"
#include "../libdetour/libdetour.h"

static detour_ctx_t move_ctx;

void HookedCL_Move(float accumulated_extra_samples, bool bFinalTick)
{
	features::ticks.Run(accumulated_extra_samples, bFinalTick);
}

void HookCL_Move(void)
{
	detour_init(&move_ctx, Sigs::CL_Move.GetPointer(), (void *)&HookedCL_Move);

	if (!detour_enable(&move_ctx))
	{
		interfaces::Cvar->ConsolePrintf("Couldn't hook CL_Move\n");
		return;
	}

#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("CL_Move hooked\n");
#endif
}
