#include "isurface_setcursor.h"

#include "../sdk/interfaces/interfaces.h"
#include "../settings/settings.h"

#include "../hooks.h"

using ISurface_LockCursorFn = void (*)(void* rdi);
static void ISurface_LockCursor(void* rdi)
{
	if (Settings::menu_open)
		return interfaces::Surface->UnlockCursor();

	auto original = VMTHooks::Surface.GetOriginal<ISurface_LockCursorFn>(62);
	original(rdi);
}

using ISurface_SetCursorFn = void (*)(void* rdi, HCursor cursor);
static void ISurface_SetCursor(void* rdi, HCursor cursor)
{
	if (Settings::menu_open)
	{
		switch (cursor)
		{
		case 0:
			cursor = 2;
			break;
		case 1:
			cursor = 3;
			break;
		case 2:
			cursor = 12;
			break;
		case 3:
			cursor = 11;
			break;
		case 4:
			cursor = 10;
			break;
		case 5:
			cursor = 9;
			break;
		case 6:
			cursor = 8;
			break;
		case 7:
			cursor = 14;
			break;
		case 8:
			cursor = 13;
			break;
		}
	}

	auto original = VMTHooks::Surface.GetOriginal<ISurface_SetCursorFn>(51);
	original(rdi, cursor);
}

void HookLockCursor()
{
	// lockcursor
	VMTHooks::Surface.Hook(62, &ISurface_LockCursor);

	// setcursor
	VMTHooks::Surface.Hook(51, &ISurface_SetCursor);
}