#pragma once

#include "../vtables.h"
#include "../sdk/interfaces/interfaces.h"
#include "../settings.h"

DECLARE_VTABLE_HOOK(ISurface_LockCursor, void, (ISurface* thisptr))
{
	if (settings.menu_open)
		interfaces::Surface->UnlockCursor();
	else
		originalISurface_LockCursor(thisptr);
}

inline void HookLockCursor()
{
	INSTALL_VTABLE_HOOK(ISurface_LockCursor, interfaces::Surface, 62);
}