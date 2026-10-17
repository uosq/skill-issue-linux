#include "../sdk/interfaces/interfaces.h"

#include "../features/misc/misc.h"
#include "../features/hookmanager/hookmanager.hpp"

INIT_VTABLE_HOOK(SleepUntilInput, void, (void* rdi, int nMaxSleepTimeMS), interfaces::InputSystem, 31)
{
	if (config::no_engine_sleep::enabled.Get())
		return;

	original_SleepUntilInput(rdi, nMaxSleepTimeMS);
}