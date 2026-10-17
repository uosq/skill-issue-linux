#include "../features/config/config.h"
#include "../features/hookmanager/hookmanager.hpp"

/*
Inside CMatSystemSurface::LockCursor (vtable index 62) it does this:
void __thiscall CMatSystemSurface::vfunction62(void)
{
	::LockCursor(0); <- we want this one
	return;
}
*/
INIT_HOOK(LockCursor, void, (void* rdi, bool state), "vguimatsurface.so", "40 88 3D ? ? ? ? E9")
{
	if (menu_open.Get())
		return original_LockCursor(rdi, false); // <- UnlockCursor();

	original_LockCursor(rdi, state);
}