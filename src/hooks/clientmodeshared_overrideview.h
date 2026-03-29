#pragma once

#include "../sdk/interfaces/interfaces.h"
#include "../features/entitylist/entitylist.h"
#include "../sdk/classes/entity.h"
#include "../sdk/classes/player.h"
#include "../sdk/helpers/helper.h"
#include "../settings/settings.h"
#include <string>

#include "../features/visuals/customfov/customfov.h"
#include "../features/visuals/norecoil/norecoil.h"
#include "../features/visuals/thirdperson/thirdperson.h"

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

static void AS_OverrideView_Callback(CViewSetup* pView)
{
	std::vector<ASHook> hooks;
	if (!Hooks_GetHooks("OverrideView", hooks))
		return;

	auto engine = API::GetScriptEngine();

	for (const auto& hook : hooks)
	{
		asIScriptContext* ctx = engine->RequestContext();

		ctx->Prepare(hook.func);
		ctx->SetArgObject(0, pView);
		ctx->Execute();

		engine->ReturnContext(ctx);
	}
}

DECLARE_VTABLE_HOOK(OverrideView, void, (IClientMode *thisptr, CViewSetup *pView))
{
	originalOverrideView(thisptr, pView);

	if (pView == nullptr)
		return;

	AS_OverrideView_Callback(pView);

	if (CTFPlayer* pLocal = EntityList::GetLocal(); pLocal != nullptr)
	{
		Thirdperson::OverrideView(pLocal, pView);

		if (!pLocal->IsAlive())
			return;

		NoRecoil::RunOverrideView(pLocal, pView);
		CustomFov::Run(pLocal, pView);
	}
}

inline void HookOverrideView()
{
	INSTALL_VTABLE_HOOK(OverrideView, interfaces::ClientMode, 17);

	#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("ClientModeShared::OverrideView hooked\n", color);
	#endif
}