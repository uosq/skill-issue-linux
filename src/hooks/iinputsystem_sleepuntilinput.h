#pragma once

#include "../vtables.h"

#include "../sdk/helpers/helper.h"
#include "../settings/settings.h"

DECLARE_VTABLE_HOOK(SleepUntilInput, void, (void *self, int nMaxSleepTimeMS))
{
	if (Settings::Misc.no_engine_sleep)
		return;

	originalSleepUntilInput(self, nMaxSleepTimeMS);
}

static void Hook_SleepUntilInput()
{
	INSTALL_VTABLE_HOOK(SleepUntilInput, interfaces::InputSystem, 31);
}