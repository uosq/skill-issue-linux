#include "../core/core.h"

#include "../sdk/definitions/igameevents.h"
#include "../sdk/signatures/signatures.h"

#include "../thirdparty/libdetour/libdetour.h"

#include "../features/scriptmanager/scriptmanager.h"
#include "../features/hook_initializer/initializer.h"

ADD_SIG(ClientModeShared_FireGameEvent, "client.so",
	"55 48 89 E5 41 57 41 56 49 89 FE 41 55 41 54 49 89 F4 53 48 8D 35")

// Ideally instead of a hook i should just get ClientModeShared and use it instead

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

MARK_FOR_INIT(HookFireGameEvent)