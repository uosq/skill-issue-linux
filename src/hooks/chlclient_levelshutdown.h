#pragma once

#include "../vtables.h"

#include "../sdk/helpers/helper.h"

#include "../features/entitylist/entitylist.h"
#include "../features/warp/warp.h"
#include "../features/backtrack/backtrack.h"
#include "../features/aimbot/projectile/projectile.h"
#include "../features/bhop/bhop.h"

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

static void AS_LevelShutdown_Callback()
{
	std::vector<ASHook> hooks;
	if (!Hooks_GetHooks("LevelShutdown", hooks))
		return;

	auto engine = API::GetScriptEngine();

	for (const auto& hook : hooks)
	{
		asIScriptContext* ctx = engine->RequestContext();

		ctx->Prepare(hook.func);
		ctx->Execute();

		engine->ReturnContext(ctx);
	}
}

DECLARE_VTABLE_HOOK(LevelShutdown, void, (CHLClient* thisptr))
{
	EntityList::Clear();
	Warp::Reset();
	Backtrack::Reset();
	gAimProjectile.Reset();
	gAimProjectile.ResetIndicator();
	Bhop::Reset();

	AS_LevelShutdown_Callback();
	originalLevelShutdown(thisptr);
}

static void HookLevelShutdown()
{
	INSTALL_VTABLE_HOOK(LevelShutdown, interfaces::ClientDLL, 7);

	#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("BaseClientDll::LevelShutdown hooked\n", color);
	#endif
}