#include "iinputsystem_sleepuntilinput.h"

#include "../hooks.h"

#include "../settings/settings.h"

using SleepUntilInputFn = void (*)(void* rdi, int nMaxSleepTimeMS);

static void SleepUntilInput(void* rdi, int nMaxSleepTimeMS)
{
	if (Config.misc.packed.no_engine_sleep)
		return;

	auto original = VMTHooks::InputSystem.GetOriginal<SleepUntilInputFn>(31);

	original(rdi, nMaxSleepTimeMS);
}

void Hook_SleepUntilInput()
{
	VMTHooks::InputSystem.Hook(31, &SleepUntilInput);
}