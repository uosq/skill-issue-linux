#pragma once

#include "../vtables.h"
#include "../sdk/interfaces/interfaces.h"
#include "../settings.h"

#include "../imgui/imgui.h"

DECLARE_VTABLE_HOOK(ISurface_LockCursor, void, (void* thisptr))
{
	if (g_Settings.menu_open)
		return interfaces::Surface->UnlockCursor();

	originalISurface_LockCursor(thisptr);
}

DECLARE_VTABLE_HOOK(ISurface_SetCursor, void, (void* thisptr, HCursor cursor))
{
	if (g_Settings.menu_open)
	{
		switch (cursor)
		{
			case 0: cursor = 2; break;
			case 1: cursor = 3; break;
			case 2: cursor = 12; break;
			case 3: cursor = 11; break;
			case 4: cursor = 10; break;
			case 5: cursor = 9; break;
			case 6: cursor = 8; break;
			case 7: cursor = 14; break;
			case 8: cursor = 13; break;
		}
	}

	originalISurface_SetCursor(thisptr, cursor);
}

inline void HookLockCursor()
{
	INSTALL_VTABLE_HOOK(ISurface_LockCursor, interfaces::Surface, 62);
	INSTALL_VTABLE_HOOK(ISurface_SetCursor, interfaces::Surface, 51);
}