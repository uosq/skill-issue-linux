#include "../core/core.h"

#include "../hooks.h"
#include "../settings/settings.h"

#include "../features/hook_initializer/initializer.h"

using SleepUntilInputFn = void (*)(void* rdi, int nMaxSleepTimeMS);

static void SleepUntilInput(void* rdi, int nMaxSleepTimeMS)
{
	if (gApp->IsInitialized())
	{
		if (Config.misc.packed.no_engine_sleep)
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