#include "cl_move.h"

#include "../features/ticks/ticks.h"
#include "../thirdparty/libdetour/libdetour.h"
#include "../core/core.h"

static detour_ctx_t move_ctx;
DETOUR_DECL_TYPE(void, CL_Move, float, float);

void HookedCL_Move(float accumulated_extra_samples, bool bFinalTick)
{
	if (gApp->IsInitialized())
	{
		features::ticks.Run(accumulated_extra_samples, bFinalTick);
		return;
	}

	DETOUR_ORIG_CALL(&move_ctx, CL_Move, accumulated_extra_samples, bFinalTick);
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
