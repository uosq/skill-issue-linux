#pragma once

#include "../sdk/interfaces/interfaces.h"
#include "../settings/settings.h"
#include "../libdetour/libdetour.h"
#include <cstdlib>

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

inline detour_ctx_t shutdownctx;
DETOUR_DECL_TYPE(void, originalHost_ShutdownFn, void);

static void HookedHost_ShutdownFn(void)
{
	std::vector<ASHook> hooks;
	if (Hooks_GetHooks("GameShutdown", hooks))
	{
		auto engine = API::GetScriptEngine();

		for (const auto& hook : hooks)
		{
			asIScriptContext* ctx = engine->RequestContext();

			ctx->Prepare(hook.func);
			ctx->Execute();

			engine->ReturnContext(ctx);
		}
	}

	DETOUR_ORIG_CALL(&shutdownctx, originalHost_ShutdownFn);
}

static void HookHost_Shutdown()
{
	void* original = sigscan_module("engine.so", "80 3D ? ? ? ? 00 0F 85 ? ? ? ? 55 31 F6");
	detour_init(&shutdownctx, original, (void*)&HookedHost_ShutdownFn);
	detour_enable(&shutdownctx);

	#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("Host_Shutdown hooked\n");
	#endif
}