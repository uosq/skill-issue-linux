#pragma once

// wtf is this pPlayer?
// IClientMode::ShouldDrawLocalPlayer(CTFPlayer* pPlayer);
// index: 14

#include "../sdk/classes/player.h"
#include "../settings/settings.h"
#include "../vtables.h"

#include "../features/entitylist/entitylist.h"

DECLARE_VTABLE_HOOK(ShouldDrawViewModel, bool, (IClientMode * self))
{
	//if (Settings::Misc.no_zoom)
	return true;

	return originalShouldDrawViewModel(self);
}

static void Hook_ShouldDrawViewModel()
{
	INSTALL_VTABLE_HOOK(ShouldDrawViewModel, interfaces::ClientMode, 25);
}