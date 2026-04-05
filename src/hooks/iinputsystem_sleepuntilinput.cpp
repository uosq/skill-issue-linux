#include "iinputsystem_sleepuntilinput.h"

#include "../vtables.h"

#include "../sdk/interfaces/interfaces.h"
#include "../settings/settings.h"

DECLARE_VTABLE_HOOK(SleepUntilInput, void, (void *self, int nMaxSleepTimeMS))
{
	if (Settings::Misc.no_engine_sleep)
		return;

	originalSleepUntilInput(self, nMaxSleepTimeMS);
}

void Hook_SleepUntilInput()
{
	INSTALL_VTABLE_HOOK(SleepUntilInput, interfaces::InputSystem, 31);
}