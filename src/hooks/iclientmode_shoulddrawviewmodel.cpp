#include "iclientmode_shoulddrawviewmodel.h"

#include "../sdk/interfaces/interfaces.h"

#include "../vtables.h"

DECLARE_VTABLE_HOOK(ShouldDrawViewModel, bool, (IClientMode * self))
{
	return true;
	//return originalShouldDrawViewModel(self);
}

void Hook_ShouldDrawViewModel()
{
	INSTALL_VTABLE_HOOK(ShouldDrawViewModel, interfaces::ClientMode, 25);
}