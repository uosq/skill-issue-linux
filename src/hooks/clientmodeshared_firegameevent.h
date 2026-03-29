#pragma once

#include "../sdk/definitions/igameevents.h"
#include "../libdetour/libdetour.h"
#include "../libsigscan.h"

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

// Ideally instead of a hook i should just get ClientModeShared and use it instead

inline detour_ctx_t firegameevent_ctx;
DETOUR_DECL_TYPE(void, original_FireGameEvent, void* self, IGameEvent* gameEvent);

static void AS_FireGameEvent_Callback(IGameEvent* event)
{
	std::vector<ASHook> hooks;
	if (!Hooks_GetHooks("FireGameEvent", hooks))
		return;

	auto engine = API::GetScriptEngine();

	for (const auto& hook : hooks)
	{
		asIScriptContext* ctx = engine->RequestContext();

		ctx->Prepare(hook.func);
		ctx->SetArgObject(0, event);
		ctx->Execute();

		engine->ReturnContext(ctx);
	}
}

inline void Hooked_FireGameEvent(void* self, IGameEvent* event)
{
	if (event == nullptr)
	{
		DETOUR_ORIG_CALL(&firegameevent_ctx, original_FireGameEvent, self, event);
		return;
	}

	AS_FireGameEvent_Callback(event);

	DETOUR_ORIG_CALL(&firegameevent_ctx, original_FireGameEvent, self, event);
}

inline void HookFireGameEvent(void)
{
	// xref: #game_player_joined_game
	void* original = (sigscan_module("client.so", "55 48 89 E5 41 57 41 56 49 89 FE 41 55 41 54 49 89 F4 53 48 8D 35"));
	detour_init(&firegameevent_ctx, original, (void*)&Hooked_FireGameEvent);
	detour_enable(&firegameevent_ctx);

	#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("ClientModeShared::FireGameEvent hooked\n");
	#endif
}