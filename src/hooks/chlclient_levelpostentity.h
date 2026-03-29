#pragma once

#include "../vtables.h"
#include "../sdk/helpers/helper.h"
#include "../features/entitylist/entitylist.h"
#include "../features/visuals/viewmodel_aim/viewmodel_aim.h"

#if 0
#include "../features/lua/hookmgr.h"
#include "../features/lua/api.h"
#include "../features/lua/classes.h"
#endif

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

static void AS_LevelInitPostEntity_Callback()
{
	std::vector<ASHook> hooks;
	if (!Hooks_GetHooks("LevelInitPostEntity", hooks))
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

DECLARE_VTABLE_HOOK(LevelInitPostEntity, void, (CHLClient* thisptr))
{
	EntityList::Reserve();
	ViewmodelAim::ResetStopTime();

	AS_LevelInitPostEntity_Callback();
	originalLevelInitPostEntity(thisptr);
}

static void HookLevelInitPostEntity()
{
	INSTALL_VTABLE_HOOK(LevelInitPostEntity, interfaces::ClientDLL, 6);

	#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("BaseClientDll::LevelInitPostEntity hooked\n", color);
	#endif
}