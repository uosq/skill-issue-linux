#include "../core/core.h"

#include "../hooks.h"

#include "../features/misc/misc.h"
#include "../features/hook_initializer/initializer.h"

using SleepUntilInputFn = void (*)(void* rdi, int nMaxSleepTimeMS);

static void SleepUntilInput(void* rdi, int nMaxSleepTimeMS)
{
	if (gApp->IsInitialized())
	{
		if (config::no_engine_sleep::enabled.Get())
			return;
	}

	auto original = VMTHooks::InputSystem.GetOriginal<SleepUntilInputFn>(31);
	original(rdi, nMaxSleepTimeMS);
}

void Hook_SleepUntilInput()
{
	VMTHooks::InputSystem.Hook(31, &SleepUntilInput);
}

MARK_FOR_INIT(Hook_SleepUntilInput)