#include "../core/core.h"

#include "../sdk/signatures/signatures.h"

#include "../thirdparty/libdetour/libdetour.h"

#include "../features/ticks/ticks.h"
#include "../features/hook_initializer/initializer.h"

ADD_SIG(CL_Move, "engine.so", "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 78 83 3D ? ? ? ? 01")

// CL_Move(float accumulated_shit, bool bFinalTick) 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 78 83 3D ? ? ? ? 01
// CL_SendMove(void) 55 66 0F EF C0 48 89 E5 41 57 41 56 48 8D BD E8 EF FF FF

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

MARK_FOR_INIT(HookCL_Move)