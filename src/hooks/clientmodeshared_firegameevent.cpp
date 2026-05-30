#include "clientmodeshared_firegameevent.h"

#include "../thirdparty/libdetour/libdetour.h"
#include "../features/scriptmanager/scriptmanager.h"

#include "../core/core.h"

DETOUR_DECL_TYPE(void, original_FireGameEvent, void *self, IGameEvent *gameEvent);
detour_ctx_t firegameevent_ctx;

void Hooked_FireGameEvent(void *self, IGameEvent *event)
{
	if (!gApp->IsInitialized() || event == nullptr)
	{
		DETOUR_ORIG_CALL(&firegameevent_ctx, original_FireGameEvent, self, event);
		return;
	}

	features::scriptmanager.CallHooks("FireGameEvent", event);

	DETOUR_ORIG_CALL(&firegameevent_ctx, original_FireGameEvent, self, event);
}

void HookFireGameEvent(void)
{
	// xref: #game_player_joined_game
	detour_init(&firegameevent_ctx, Sigs::ClientModeShared_FireGameEvent.GetPointer(),
		    (void *)&Hooked_FireGameEvent);
	detour_enable(&firegameevent_ctx);

#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("ClientModeShared::FireGameEvent hooked\n");
#endif
}