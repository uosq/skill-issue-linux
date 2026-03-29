#pragma once

#include "../vtables.h"
#include "../sdk/helpers/helper.h"
#include "../features/entitylist/entitylist.h"

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

static void AS_LevelInitPreEntity_Callback(const char* mapName)
{
	std::vector<ASHook> hooks;
	if (!Hooks_GetHooks("LevelInitPreEntity", hooks))
		return;

	auto engine = API::GetScriptEngine();

	std::string name(mapName);

	for (const auto& hook : hooks)
	{
		asIScriptContext* ctx = engine->RequestContext();

		ctx->Prepare(hook.func);
		ctx->SetArgObject(0, &name);
		ctx->Execute();

		engine->ReturnContext(ctx);
	}
}

DECLARE_VTABLE_HOOK(LevelInitPreEntity, void, (CHLClient* thisptr, const char* mapName))
{
	AS_LevelInitPreEntity_Callback(mapName);
	originalLevelInitPreEntity(thisptr, mapName);
}

static void HookLevelInitPreEntity()
{
	INSTALL_VTABLE_HOOK(LevelInitPreEntity, interfaces::ClientDLL, 5);

	#ifdef DEBUG
	constexpr Color_t color{100, 255, 100, 255};
	helper::console::ColoredPrint("BaseClientDll::LevelInitPreEntity hooked\n", color);
	#endif
}